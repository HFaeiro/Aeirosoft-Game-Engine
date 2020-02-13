#include "Collision.h"

Collidable::Collidable(graphics* g) : g(g), pDevice(g->GetDevice().Get()), pContext(g->GetDeviceContext().Get())
{
	if (g != nullptr)
	{

		DWORD Indecies[24] =
		{
			0,1,1,2,2,3,3,0,
			4,5,5,6,6,7,7,4,
			0,4,1,5,2,6,3,7
		};

		ib.Init(g->GetDevice().Get(), Indecies, 24);

		CreateTexture();

	}
}

void Collidable::TransformBounds(DirectX::XMMATRIX m)
{
	this->currentMatrix = m;
	vTransbBox.clear();
	DirectX::BoundingOrientedBox tmpBox;
	if (vOGbBox.empty())
	{
		return;
	}
	for (auto& v : vOGbBox) {
		DirectX::XMFLOAT3 corners[8];
		DirectX::XMVECTOR vCorners[8];
		DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		v.GetCorners(corners);
		for (int i = 0; i < 8; i++)
		{
			vCorners[i] = DirectX::XMLoadFloat3(&corners[i]);
			vCorners[i] = DirectX::XMVector3Transform(vCorners[i], m);
			DirectX::XMStoreFloat3(&corners[i] ,vCorners[i]);

		}
		for (const auto c : corners)
		{

			minVertex.x = std::min(minVertex.x, c.x);
			minVertex.y = std::min(minVertex.y, c.y);
			minVertex.z = std::min(minVertex.z, c.z);

			maxVertex.x = std::max(maxVertex.x, c.x);
			maxVertex.y = std::max(maxVertex.y, c.y);
			maxVertex.z = std::max(maxVertex.z, c.z);
		}
		//v.Transform(tmpBox, m);
		v.CreateFromPoints(tmpBox, 8, corners, sizeof(DirectX::XMFLOAT3));
		vTransbBox.push_back(tmpBox);

	}
	if (type == Type::Entity)
		bSphere = DirectX::BoundingSphere(tmpBox.Center, tmpBox.Extents.z);
	
}

void Collidable::DrawBoundingOrientedBox()
{

	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pContext->PSSetShaderResources(0, 1, redTexture.GetTextureResourceViewAddr());
	g->m_TextureShader.UpdateWorldMatrixBuffer(pContext, DirectX::XMMatrixIdentity());

	for (auto& v : vTransbBox) {

		std::vector<Vertex> vertices;

		DirectX::XMFLOAT3 corners[8];
		v.GetCorners(corners);

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

void Collidable::CreateBoundingOrientedBox(std::vector<Vertex> v)
{

	DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (Vertex i : v)
	{
		minVertex.x = std::min(minVertex.x, i.position.x);
		minVertex.y = std::min(minVertex.y, i.position.y);
		minVertex.z = std::min(minVertex.z, i.position.z);

		maxVertex.x = std::max(maxVertex.x, i.position.x);
		maxVertex.y = std::max(maxVertex.y, i.position.y);
		maxVertex.z = std::max(maxVertex.z, i.position.z);
	}

	if (vMin.empty()) {
		vMin.push_back(minVertex);
		vMax.push_back(maxVertex);
		CreateBoundingOrientedBox(minVertex, maxVertex);
	}
	else
	{
		vMin.push_back(minVertex);
		vMax.push_back(maxVertex);

		
		for (int i = 0 ; i < vMin.size(); i++)
		{
			minVertex.x = std::min(minVertex.x, vMin[i].x);
			minVertex.y = std::min(minVertex.y, vMin[i].y);
			minVertex.z = std::min(minVertex.z, vMin[i].z);

			maxVertex.x = std::max(maxVertex.x, vMax[i].x);
			maxVertex.y = std::max(maxVertex.y, vMax[i].y);
			maxVertex.z = std::max(maxVertex.z, vMax[i].z);
		}
		vOGbBox.clear();
		CreateBoundingOrientedBox(minVertex, maxVertex);
		

	}

}


void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& size)
{
	DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
	DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };

	CreateBoundingOrientedBox(minVertex, maxVertex);

}
void Collidable::CreateBoundingOrientedBox(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max)
{
	DirectX::XMFLOAT3 centerOffset = { .5f * (min.x + max.x), .5f * (min.y + max.y) , .5f * (min.z + max.z) };
	DirectX::XMFLOAT3 Extents = { .5f * (min.x - max.x), .5f * (min.y - max.y) , .5f * (min.z - max.z) };

	vOGbBox.push_back(DirectX::BoundingOrientedBox(centerOffset, Extents, { 0,0,0,1.f }));
	//bSphere.CreateFromBoundingBox(vOGbBox[0]);
	if(type == Type::Entity)
		bSphere = DirectX::BoundingSphere(centerOffset, Extents.z);
}

void Collidable::AddBoundingOrientedBox(DirectX::BoundingOrientedBox& bBox)
{
	vOGbBox.push_back(bBox);
}

void Collidable::AddBoundingOrientedBox(DirectX::XMFLOAT3 size)
{

	if (vOGbBox.empty())
		CreateBoundingOrientedBox(size);
	else
	{
		DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
		DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };

		DirectX::XMFLOAT3 centerOffset = { .5f * (minVertex.x + maxVertex.x), .5f * (minVertex.y + maxVertex.y) , .5f * (minVertex.z + maxVertex.z) };
		DirectX::XMFLOAT3 Extents = { .5f * (minVertex.x - maxVertex.x), .5f * (minVertex.y - maxVertex.y) , .5f * (minVertex.z - maxVertex.z) };

		vOGbBox.push_back(DirectX::BoundingOrientedBox(centerOffset, Extents, { 0,0,0,1.f }));
	}

}

void Collidable::CreateTexture()
{
	redTexture = texture(pDevice, colors::UnhandledTexturecolor, aiTextureType::aiTextureType_DIFFUSE);
}
