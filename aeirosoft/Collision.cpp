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
	DirectX::BoundingBox tmpBox;
	if (vOGbBox.empty())
	{
		return;
	}
	for (auto& v : vOGbBox) {
		v.Transform(tmpBox, m);
		vTransbBox.push_back(tmpBox);
	}

}

void Collidable::DrawBoundingBox()
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

void Collidable::CreateBoundingBox(std::vector<Vertex> v)
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
		CreateBoundingBox(minVertex, maxVertex);
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
		CreateBoundingBox(minVertex, maxVertex);
		

	}

}


void Collidable::CreateBoundingBox(DirectX::XMFLOAT3& size)
{
	DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
	DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };

	CreateBoundingBox(minVertex, maxVertex);

}
void Collidable::CreateBoundingBox(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max)
{
	DirectX::XMFLOAT3 centerOffset = { .5f * (min.x + max.x), .5f * (min.y + max.y) , .5f * (min.z + max.z) };
	DirectX::XMFLOAT3 Extents = { .5f * (min.x - max.x), .5f * (min.y - max.y) , .5f * (min.z - max.z) };

	vOGbBox.push_back(DirectX::BoundingBox(centerOffset, Extents));


}

void Collidable::AddBoundingBox(DirectX::BoundingBox& bBox)
{
	vOGbBox.push_back(bBox);
}

void Collidable::AddBoundingBox(DirectX::XMFLOAT3 size)
{

	if (vOGbBox.empty())
		CreateBoundingBox(size);
	else
	{
		DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
		DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };

		DirectX::XMFLOAT3 centerOffset = { .5f * (minVertex.x + maxVertex.x), .5f * (minVertex.y + maxVertex.y) , .5f * (minVertex.z + maxVertex.z) };
		DirectX::XMFLOAT3 Extents = { .5f * (minVertex.x - maxVertex.x), .5f * (minVertex.y - maxVertex.y) , .5f * (minVertex.z - maxVertex.z) };

		vOGbBox.push_back(DirectX::BoundingBox(centerOffset, Extents));
	}

}

void Collidable::CreateTexture()
{
	redTexture = texture(pDevice, colors::UnhandledTexturecolor, aiTextureType::aiTextureType_DIFFUSE);
}
