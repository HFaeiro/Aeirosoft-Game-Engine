#include "Collision.h"

Collidable::Collidable(graphics* g) : g(g), pDevice(g->GetDevice().Get()), pContext(g->GetDeviceContext().Get())
{
	if (g != nullptr)
	{
#ifdef _DEBUG

		Indecies.insert(Indecies.end(), {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
			});
		ib.Init(g->GetDevice().Get(), Indecies.data(), Indecies.size());

		CreateTexture();
#endif
	}
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
			((DirectX::BoundingOrientedBox)b).GetCorners(tmpf3);
			for (int i = 0; i < 8; i++)
				corners.push_back(tmpf3[i]);
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
	pContext->PSSetShaderResources(0, 1, redTexture.GetTextureResourceViewAddr());
	g->m_TextureShader.UpdateWorldMatrixBuffer(pContext, DirectX::XMMatrixIdentity());

	std::vector<aeBounding> tmpb = GetBounds();
	Indecies.clear();
	g->Begin3DScene();
	for (auto& v : tmpb) {


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
#endif
void Collidable::CreateBoundingOrientedBox(std::vector<std::vector<Vertex>> v, std::vector<DirectX::XMMATRIX*> transforms)
{
	std::vector<Vertex> tmpV;
	for (const auto& vertexGroup : v)
		for (const auto& vertice : vertexGroup)
			tmpV.push_back(vertice);
	aeBounding tmpBounds;
	if (tmpBounds.Create(tmpV))
		Boundings.push_back(tmpBounds);
	hasMainBox = true;
	int i = 0;
	for (const auto& vertexGroup : v)
	{
		aeBounding tmpBounds;
		if (tmpBounds.Create(vertexGroup))
		{
			if (transforms[i] != nullptr)
				tmpBounds.setTransformPtr(transforms[i]);
			Boundings.push_back(tmpBounds);
			
		}
		i++;
	}
}
void Collidable::CreateBoundingOrientedBox(std::vector<Vertex> v)
{
	aeBounding tmpBounds;
	if (tmpBounds.Create(v))
		Boundings.push_back(tmpBounds);
}
//void Collidable::CreateBoundingOrientedBox(std::vector<std::vector<Vertex>> v)
//{
//	std::vector<Vertex> tmpV;
//	for (const auto& vertexGroup : v)
//		for(const auto& vertice : vertexGroup)
//			tmpV.push_back(vertice);
//	CreateBoundingOrientedBox(tmpV);
//	if(vOGbBox.size())
//		MainBox = vOGbBox[0];
//	for (const auto& vertexGroup : v)
//		CreateBoundingOrientedBox(vertexGroup);
//}



void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& size)
{

	aeBounding tmpBounds;
	if (tmpBounds.CreateFromFloat3(size))
		Boundings.push_back(tmpBounds);

}
//void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max)
//{
//	DirectX::XMFLOAT3 centerOffset = { .5f * (min.x + max.x), .5f * (min.y + max.y) , .5f * (min.z + max.z) };
//	DirectX::XMFLOAT3 Extents = { .5f * (min.x - max.x), .5f * (min.y - max.y) , .5f * (min.z - max.z) };
//
//	vOGbBox.push_back(DirectX::BoundingOrientedBox(centerOffset, Extents, { 0,0,0,1.f }));
//	//bSphere.CreateFromBoundingBox(vOGbBox[0]);
//	if(type == Type::Entity)
//		bSphere = DirectX::BoundingSphere(centerOffset, Extents.z);
//}

//void Collidable::AddBoundingOrientedBox(DirectX::BoundingOrientedBox& bBox)
//{
//	vOGbBox.push_back(bBox);
//}
//
//void Collidable::AddBoundingOrientedBox(DirectX::XMFLOAT3 size)
//{
//
//	if (vOGbBox.empty())
//		CreateBoundingOrientedBox(size);
//	else
//	{
//		DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
//		DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };
//
//		DirectX::XMFLOAT3 centerOffset = { .5f * (minVertex.x + maxVertex.x), .5f * (minVertex.y + maxVertex.y) , .5f * (minVertex.z + maxVertex.z) };
//		DirectX::XMFLOAT3 Extents = { .5f * (minVertex.x - maxVertex.x), .5f * (minVertex.y - maxVertex.y) , .5f * (minVertex.z - maxVertex.z) };
//
//		vOGbBox.push_back(DirectX::BoundingOrientedBox(centerOffset, Extents, { 0,0,0,1.f }));
//	}
//
//}

void Collidable::CreateTexture()
{
	redTexture = texture(pDevice, colors::UnhandledTexturecolor, aiTextureType::aiTextureType_DIFFUSE);
}



////Collision/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::optional<Events*> Collision::Queue()
{

	for (const auto& C : collidable)
	{
#ifdef _DEBUG
		C->DrawBoundingOrientedBox();
#endif
		if (C->type != Collidable::Type::Object)
		{
			if (C->CheckRay)
			{
				float f = 0.f;
				for (const auto& c : collidable)
				{
					if (c != C)
					{

						std::vector<aeBounding> bounds = c->GetBounds();
						if (c->hasMainBox)
							if (((DirectX::BoundingOrientedBox)bounds[0]).Intersects(C->clickOrigin, C->clickDestination, f))
								bounds.erase(bounds.begin());
							else
								bounds.clear();
						for (const auto& box : bounds)
							if (((DirectX::BoundingOrientedBox)box).Intersects(C->clickOrigin, C->clickDestination, f))
							{
								c->hit = true;
								break;
							}
					}
				}
				C->CheckRay = false;
			}
			if(!C->collision)
				Check(C);
		}
	}
	return {};
}

void Collision::Check(Collidable* C)
{
	for (const auto& c : collidable)
	{
		if (c != C)
		{
			if (C->type != Collidable::Type::Object) {
				auto box = C->GetBoundSphere();
				auto bounds = c->GetBounds();
				if (c->hasMainBox)
					if (box.Contains(((DirectX::BoundingOrientedBox)bounds[0])))
					{

						bounds.erase(bounds.begin());

					}
					else
						continue;
 
				for (const auto& bound : bounds)
					if (box.Contains(((DirectX::BoundingOrientedBox)bound)))
					{
						if (c->type != Collidable::Type::EntityAi || C->type == Collidable::Type::EntityAi)
						{
							C->collision = true;
							break;
						}
						else
						{
							C->hit = true;
							break;
						}
					}
			}
		}
	}
}
