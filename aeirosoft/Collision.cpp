#include "Collision.h"
#include <thread>
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

		if (v.collision)
			tex = &redTexture;
		else
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
	if (collidable.size())
	{
		pDevice = collidable[0]->pDevice;
		pContext = collidable[0]->pContext;
	}
	else
		return false;
	//Create Quad Tree now that everything should be added to the collidable Vector
	std::vector < DirectX::XMFLOAT3 > corners;
	DirectX::XMFLOAT3 tmpf3[8];

	//first we have to get all our corners and create a box around the whole thing. 
	for (const auto& object : collidable)
	{
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
	for (auto& object : collidable)
	{
		for (auto& bound : object->Boundings)
		{
			int i = 0;
			for (const auto& quad : worldQuad)
			{
				if (quad.Contains(static_cast<DirectX::BoundingOrientedBox>(bound)))
					bound.QuadID.emplace_back(i);
					i++;
			}
		}

	}





	return true;
}

////Collision/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::optional<Events*> Collision::Queue()
{

	for (auto& C : collidable)
	{

		//C->Boundings[0].vAdjustments.clear();
		if (C->type != Collidable::Type::Object)
		{

			C->Boundings[0].QuadID.clear();
			int i = 0;
			for (const auto& quad : worldQuad)
			{
				if (quad.Contains(static_cast<DirectX::BoundingSphere>(C->Boundings[0])))
					C->Boundings[0].QuadID.push_back(i);
				i++;
			}
			if (!C->Boundings[0].QuadID.size())
				continue;

			if (C->CheckRay)
			{
				float f = 0.f;
				for (const auto& c : collidable)
				{
					if (c != C && c->type != Collidable::Type::Object)
					{

						std::vector<aeBounding> bounds = c->GetBounds();
						if (c->hasMainBox || bounds.size() == 1)
							if (((DirectX::BoundingOrientedBox)bounds[0]).Intersects(C->clickOrigin, C->clickDestination, f))
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
			}
			//std::thread(&Collision::Check, this, std::ref(C)).detach();

			Check(C);
			//auto t1 = 

			//threads.emplace_back(std::thread(&Collision::Check, this, std::ref(C)));
		}
#ifdef _DEBUG
		C->DrawBoundingOrientedBox();
#endif
	}
	DrawBoundingQuads();
	return {};
}

void Collision::Check(Collidable* C)
{
	auto box = C->GetBoundSphere();
	for (const auto& c : collidable)
	{
		if (c != C)
		{
			if (C->type != Collidable::Type::Object) {

				auto bounds = c->GetBounds();

				if (c->hasMainBox || bounds.size() == 1)
				{
					for (const auto& qID : bounds[0].QuadID)
						for (const auto& myqID : C->Boundings[0].QuadID)
							if (qID == myqID) {
								auto bound = (bounds[0].hasSphere ? bounds[0].getSphere() : bounds[0]);
								if (box.Contains(bound))
								{
#ifdef _DEBUG
									c->Boundings[0].collision = true;
									C->Boundings[0].collision = true;
#endif // _DEBUG
									if (c->type != Collidable::Type::EntityAi && C->type == Collidable::Type::EntityAi)
									{
										c->hit = true;
									}
									if (C->resolve) {

										//threads.emplace_back(std::thread(&aeBounding::Resolve, C->Boundings[0], std::ref(bounds[0])));
										//t1.join();
										C->Boundings[0].Resolve(bounds[0]);

									}
									continue;
								}
#ifdef _DEBUG
								else
								{
									c->Boundings[0].collision = false;
									continue;
								}
#endif // _DEBUG
							}
				}
				for (const auto& bound : bounds)
					for (const auto& qID : bound.QuadID)
						for (const auto& myqID : C->Boundings[0].QuadID)
							if (qID == myqID)
								if (box.Contains((DirectX::BoundingOrientedBox)bound))
									if (C->resolve) {
										C->Boundings[0].Resolve(bound);
										continue;
									}
			}
		}
	}

}
std::vector<DirectX::BoundingBox> Collision::Subdivide(DirectX::BoundingBox box)
{
	std::vector < DirectX::XMFLOAT3 > corners;
	DirectX::XMFLOAT3 tmpf3[8];
	std::vector<DirectX::BoundingBox> quad;
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

	corners.emplace_back(maxX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, 0);
	corners.emplace_back(0, minY, 0);
	corners.emplace_back(maxX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, minZ);
	corners.emplace_back(0, minY, minZ);

	DirectX::BoundingBox quarterWorld;
	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(minX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, 0);
	corners.emplace_back(0, minY, 0);
	corners.emplace_back(minX, maxY, 0);
	corners.emplace_back(minX, minY, 0);
	corners.emplace_back(0, maxY, minZ);
	corners.emplace_back(0, minY, minZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(minX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, 0);
	corners.emplace_back(0, minY, 0);
	corners.emplace_back(minX, maxY, 0);
	corners.emplace_back(minX, minY, 0);
	corners.emplace_back(0, maxY, maxZ);
	corners.emplace_back(0, minY, maxZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();

	corners.emplace_back(maxX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, 0);
	corners.emplace_back(0, minY, 0);
	corners.emplace_back(maxX, maxY, 0);
	corners.emplace_back(maxX, minY, 0);
	corners.emplace_back(0, maxY, maxZ);
	corners.emplace_back(0, minY, maxZ);

	quarterWorld.CreateFromPoints(quarterWorld, corners.size(), corners.data(), sizeof(DirectX::XMFLOAT3));
	quad.emplace_back(quarterWorld);
	corners.clear();
	return quad;
}
void Collision::DrawBoundingQuads()
{
	if (!collidable.size())
		return;

	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	texture* tex = &collidable[0]->redTexture;

	collidable[0]->g->m_TextureShader.UpdateWorldMatrixBuffer(pContext, DirectX::XMMatrixIdentity());
	collidable[0]->g->Begin3DScene();
	for (auto& q : worldQuad) {


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
		pContext->IASetIndexBuffer(collidable[0]->ib.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->DrawIndexed(24, 0, 0);
	}
}