#include "Mesh.h"

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices, std::vector<ULONG>& indices, std::vector<texture> vTexture)
{
	this->vTexture = vTexture;
	this->pContext = pContext;
	if (FAILED(this->vertexBuffer.Init(pDevice, vertices.data(), vertices.size())))
		throw;
	if(FAILED(this->indexBuffer.Init(pDevice, indices.data(), indices.size())))
		throw;

}

Mesh::Mesh(const Mesh& mesh)
{
	this->pContext = mesh.pContext;
	this->indexBuffer = mesh.indexBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->vTexture = mesh.vTexture;
}

void Mesh::Draw()
{
	UINT offset = 0;

	for (int i = 0; i < vTexture.size(); i++)
	{
		if (vTexture[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			this->pContext->PSSetShaderResources(0, 1, vTexture[i].GetTextureResourceViewAddr());
			break;
		}
	}

	pContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.GetStridePtr(), &offset);
	pContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->DrawIndexed(this->indexBuffer.Size(), 0, 0);


}
