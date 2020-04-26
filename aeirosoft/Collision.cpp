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
					corners.push_back(tmpf3[i]);
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

void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& size)
{

	aeBounding tmpBounds;
	if (tmpBounds.CreateFromFloat3(size))
		Boundings.push_back(tmpBounds);

}

////Collision/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::optional<Events*> Collision::Queue()
{

	for (const auto& C : collidable)
	{

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
						if (c->hasMainBox || bounds.size() == 1)
							if (((DirectX::BoundingOrientedBox)bounds[0]).Intersects(C->clickOrigin, C->clickDestination, f))
								bounds.erase(bounds.begin());
							else
								continue;
						for (const auto& box : bounds)
							if (((DirectX::BoundingOrientedBox)box).Intersects(C->clickOrigin, C->clickDestination, f))
							{
								c->hit = true;
								C->CheckRay = false;
								break;
							}
					}
				}
			}
			Check(C);
		}
#ifdef _DEBUG
		C->DrawBoundingOrientedBox();
#endif
	}
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
					if (box.Contains(((DirectX::BoundingOrientedBox)bounds[0])))
					{
#ifdef _DEBUG
						c->Boundings[0].collision = true;
						C->Boundings[0].collision = true;
#endif // _DEBUG
						if (c->type != Collidable::Type::EntityAi && C->type == Collidable::Type::EntityAi)
						{
							c->hit = true;
						}
						if (C->resolve)
							C->Boundings[0].Resolve(bounds[0]);
						continue;
					}
#ifdef _DEBUG
					else
					{
						c->Boundings[0].collision = false;
						continue;
					}
#endif // _DEBUG
				for (auto& bound : bounds)
					if (box.Contains(((DirectX::BoundingOrientedBox)bound)))
						if (C->resolve)
							C->Boundings[0].Resolve(bound);
			}
		}
	}
}
