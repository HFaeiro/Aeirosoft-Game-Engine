#include "Collision.h"
Collidable::Collidable(graphics* g) : g(g), pDevice(g->GetDevice().Get()), pContext(g->GetDeviceContext().Get())
{
#ifdef _DEBUG

	if (g != nullptr)
	{

		Indecies.insert(Indecies.end(), {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
			});
		ib.Init(g->GetDevice().Get(), Indecies.data(), Indecies.size());

		CreateTexture();

	}
#endif
}
void Collidable::TransformBounds(DirectX::XMMATRIX m)
{
	if (Boundings.empty())
		return;
	std::vector < DirectX::XMFLOAT3 > corners;
	DirectX::XMFLOAT3 tmpf3[8];

	for (auto& b : Boundings) {
		b.Transform(m, type != Collidable::Type::Object);
		if (hasMainBox) {
			if (&b != &Boundings[0]) {
				((DirectX::BoundingOrientedBox)b).GetCorners(tmpf3);
				for (int i = 0; i < 8; i++)
					corners.emplace_back(tmpf3[i]);
			}
		}
	}

	if (hasMainBox)
	{
		DirectX::BoundingOrientedBox tmpBox; 
		((DirectX::BoundingOrientedBox)Boundings[0]).CreateFromPoints(tmpBox,
			corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
		Boundings[0] = tmpBox;
	}
}

#ifdef _DEBUG

void Collidable::DrawBoundingOrientedBox()
{

	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	texture* tex = nullptr;

	g->m_TextureShader.UpdateWorldMatrixBuffer(pContext, DirectX::XMMatrixIdentity());

	std::vector<aeBounding> tmpb = GetBounds();
	Indecies.clear();
	g->Begin3DScene();
	for (auto& v : tmpb) {

		/*if (v.collision)
			tex = &redTexture;
		else*/
			tex = &blueTexture;

		pContext->PSSetShaderResources(0, 1, tex->GetTextureResourceViewAddr());
		std::vector<Vertex> vertices;
		DirectX::XMFLOAT3 corners[8];
		((DirectX::BoundingOrientedBox)v).GetCorners(corners);

		for (DirectX::XMFLOAT3 j : corners)
		{
			vertices.emplace_back(j, DirectX::XMFLOAT2(0, 0));
		}

		VertexBuffer vb;
		vb.Init(pDevice, vertices.data(), vertices.size());
		pContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.GetStridePtr(), &offset);
		pContext->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->DrawIndexed(24, 0, 0);

	}
}
void Collidable::CreateTexture()
{
	redTexture = texture(pDevice, colors::UnhandledTexturecolor, aiTextureType::aiTextureType_DIFFUSE);
	blueTexture = texture(pDevice, colors::BlueTexturecolor, aiTextureType::aiTextureType_DIFFUSE);
}

#endif
void Collidable::CreateBoundingOrientedBox(std::vector<std::vector<Vertex>> v, std::vector<DirectX::XMMATRIX*> transforms)
{
	Boundings.clear();
	std::vector<Vertex> tmpV;
	for (const auto& vertexGroup : v)
		for (const auto& vertice : vertexGroup)
			tmpV.emplace_back(vertice);
	aeBounding tmpBounds;
	if (tmpBounds.Create(tmpV))
		Boundings.emplace_back(tmpBounds);
	hasMainBox = true;
	int i = 0;
	for (const auto& vertexGroup : v)
	{
		aeBounding tmpBounds;
		if (tmpBounds.Create(vertexGroup))
		{
			if (transforms.size())
				tmpBounds.setTransformPtr(transforms[i]);
			Boundings.emplace_back(tmpBounds);
		}
		i++;
	}
}
void Collidable::CreateBoundingOrientedBox(std::vector<Vertex> v)
{

	aeBounding tmpBounds;
	if (tmpBounds.Create(v))
		Boundings.emplace_back(tmpBounds);
}

void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& size)
{

	aeBounding tmpBounds;
	if (tmpBounds.CreateFromFloat3(size))
		Boundings.emplace_back(tmpBounds);

}

bool Collision::Initialize()
{
	if (staticCollidable.size())
	{
		pDevice = staticCollidable[0]->pDevice;
		pContext = staticCollidable[0]->pContext;
	}
	else
		return false;
	//Create Quad Tree now that everything should be added to the collidable Vector
	std::vector < DirectX::XMFLOAT3 > corners;
	DirectX::XMFLOAT3 tmpf3[8];

	//first we have to get all our corners and create a box around the whole thing. 
	for (const auto& object : staticCollidable)
	{
		if (object->type == Collidable::Type::Object)
		for (const auto& bound : object->Boundings)
		{
			static_cast<DirectX::BoundingOrientedBox>(bound).GetCorners(tmpf3);
			for (int i = 0; i < 8; i++)
				corners.emplace_back(tmpf3[i]);
		}

	}
	DirectX::BoundingBox world;
	if (corners.size())
		world.CreateFromPoints(world, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	else
		return false;

	corners.clear();

	worldQuad = Subdivide(world);


	FillBoundingsAndQuads();
	std::vector<QuadBox> tmpBox;
	//while ((tmpBox = Subdivide(worldQuad, 20)).size())
	//{
	//	for (const auto& box : tmpBox)
	//	{
	//		worldQuad.push_back(box);
	//	}
	//	FillBoundingsAndQuads();
	//}




	return true;
}

////Collision/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::optional<Events*> Collision::Queue()
{
	for (auto& quad : worldQuad)
	{
		quad.inside.clear();

	}

	for (auto& C : dynamicCollidable)
	{
		for (auto& quad : worldQuad)
		{
			if (quad.myBox.Intersects(static_cast<const DirectX::BoundingSphere>(C->Boundings[0])))
				quad.inside.push_back(&C->Boundings[0]);
		}
		if (C->CheckRay)
			Check(C);
#ifdef _DEBUG
		C->DrawBoundingOrientedBox();
#endif
	}
	for (auto& quad : worldQuad)
	{
		for (int i = 0; i < quad.inside.size(); i++) {
			for (const auto& object : quad.staticInside)
				quad.inside[i]->Resolve(*object);
			
			for (int j = i; j < quad.inside.size(); j++)
				quad.inside[i]->Resolve(*quad.inside[j]);
		}

	}
#ifdef _DEBUG
	DrawBoundingQuads();
#endif
	return {};
}

void Collision::Check(Collidable* C)
{
	auto mySphere = C->GetBoundSphere();
	float f = 0.f;
	for (const auto& c : dynamicCollidable)
	{
		if (C != c) {
			
				std::vector<aeBounding> bounds = c->GetBounds();
				if (c->hasMainBox || bounds.size() == 1)
					if (((const DirectX::BoundingOrientedBox)bounds[0]).Intersects(C->clickOrigin, C->clickDestination, f))
						bounds.erase(bounds.begin());
					else
						continue;
				for (const DirectX::BoundingOrientedBox& box : bounds)
					if (box.Intersects(C->clickOrigin, C->clickDestination, f))
					{
						c->hit = true;
						C->CheckRay = false;
						break;
					}
		}
	}
	C->CheckRay = false;
}
std::vector<Collision::QuadBox> Collision::Subdivide(std::vector<Collision::QuadBox>& qBox, int limit)
{
	std::vector<QuadBox> tmpBox;
	std::vector<QuadBox> tmpWorld = qBox;
	qBox.clear();

	for (const auto& quad : tmpWorld)
	{
		if (quad.inside.size() > limit)
		{
			std::vector<QuadBox> tmpBox2 = Subdivide(quad);
			for (const auto& box : tmpBox2)
				tmpBox.emplace_back(box);
		}
		else {
			qBox.emplace_back(quad);
		}
	}
	return tmpBox;
}
std::vector<Collision::QuadBox> Collision::Subdivide(DirectX::BoundingBox box) const
{
	std::vector < DirectX::XMFLOAT3 > corners;
	DirectX::XMFLOAT3 tmpf3[8];
	std::vector<Collision::QuadBox> quad;
	box.GetCorners(tmpf3);

	std::vector < DirectX::XMFLOAT3 > negXcorners;
	std::vector < DirectX::XMFLOAT3 > negXZcorners;
	std::vector < DirectX::XMFLOAT3 > negZcorners;
	float minX = FLT_MAX, minZ = FLT_MAX, minY = FLT_MAX;
	float maxX = -FLT_MAX, maxZ = -FLT_MAX, maxY = -FLT_MAX;

	for (int i = 0; i < 8; i++)
	{
		minX = std::min(minX, tmpf3[i].x);
		minZ = std::min(minZ, tmpf3[i].z);

		minY = std::min(minY, tmpf3[i].y);
		maxY = std::max(maxY, tmpf3[i].y);

		maxX = std::max(maxX, tmpf3[i].x);
		maxZ = std::max(maxZ, tmpf3[i].z);

	}
	DirectX::XMFLOAT3 c = box.Center;
	corners.emplace_back(maxX, maxY, c.z);
	corners.emplace_back(maxX, minY, c.z);
	corners.emplace_back(c.x, maxY, c.z);
	corners.emplace_back(c.x, minY, c.z);
	corners.emplace_back(maxX, maxY, minZ);
	corners.emplace_back(maxX, minY, minZ);
	corners.emplace_back(c.x, maxY, minZ);
	corners.emplace_back(c.x, minY, minZ);

	DirectX::BoundingBox quarterWorld;
	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(minX, maxY, c.z);
	corners.emplace_back(maxX, minY, c.z);
	corners.emplace_back(c.x, maxY, c.z);
	corners.emplace_back(c.x, minY, c.z);
	corners.emplace_back(minX, maxY, minZ);
	corners.emplace_back(minX, minY, minZ);
	corners.emplace_back(c.x, maxY, minZ);
	corners.emplace_back(c.x, minY, minZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(minX, maxY, c.z);
	corners.emplace_back(maxX, minY, c.z);
	corners.emplace_back(c.x, maxY, c.z);
	corners.emplace_back(c.x, minY, c.z);
	corners.emplace_back(minX, maxY, maxZ);
	corners.emplace_back(minX, minY, maxZ);
	corners.emplace_back(c.x, maxY, maxZ);
	corners.emplace_back(c.x, minY, maxZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(maxX, maxY, c.z);
	corners.emplace_back(maxX, minY, c.z);
	corners.emplace_back(c.x, maxY, c.z);
	corners.emplace_back(c.x, minY, c.z);
	corners.emplace_back(maxX, maxY, maxZ);
	corners.emplace_back(maxX, minY, maxZ);
	corners.emplace_back(c.x, maxY, maxZ);
	corners.emplace_back(c.x, minY, maxZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();
	return quad;
}
void Collision::FillBoundingsAndQuads()
{

	for (auto& object : staticCollidable)
	{
		for (auto& bound : object->Boundings)
		{
			int i = 0;
			for (auto& quad : worldQuad)
			{
				if (static_cast<DirectX::BoundingBox>(quad).Intersects(static_cast<DirectX::BoundingOrientedBox>(bound)))
				{
					//bound.QuadID.emplace_back(i);
					quad.staticInside.push_back(&bound);
				}
				i++;
			}
		}

	}
}
void Collision::DrawBoundingQuads()
{
	if (!staticCollidable.size())
		return;

	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	texture* tex = &staticCollidable[0]->redTexture;

	staticCollidable[0]->g->m_TextureShader.UpdateWorldMatrixBuffer(pContext, DirectX::XMMatrixIdentity());
	staticCollidable[0]->g->Begin3DScene();
	for (const DirectX::BoundingBox& q : worldQuad) {
		pContext->PSSetShaderResources(0, 1, tex->GetTextureResourceViewAddr());
		std::vector<Vertex> vertices;
		DirectX::XMFLOAT3 corners[8];
		q.GetCorners(corners);
		for (DirectX::XMFLOAT3 j : corners)
		{
			vertices.emplace_back(j, DirectX::XMFLOAT2(0, 0));
		}
		VertexBuffer vb;
		vb.Init(pDevice, vertices.data(), vertices.size());
		pContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.GetStridePtr(), &offset);
		pContext->IASetIndexBuffer(staticCollidable[0]->ib.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->DrawIndexed(24, 0, 0);
	}
}