#include "Mesh.h"

//Mesh::Mesh(aiMesh* mesh)
//{
//	name = helper::strings::charToWide(mesh->mName.C_Str());
//	if (mesh->mTextureCoords[0])
//		for (UINT i = 0; i < mesh->mNumVertices; i++)
//		{
//			vertices.emplace_back(mesh->mVertices[i].x,
//				mesh->mVertices[i].y,
//				mesh->mVertices[i].z,
//				(float)mesh->mTextureCoords[0][i].x,
//				(float)mesh->mTextureCoords[0][i].y
//			);
//		}
//	else
//	{
//		for (UINT i = 0; i < mesh->mNumVertices; i++)
//		{
//			vertices.push_back(Vertex(mesh->mVertices[i].x,
//				mesh->mVertices[i].y,
//				mesh->mVertices[i].z));
//		}
//	}
//
//	for (UINT i = 0; i < mesh->mNumFaces; i++)
//	{
//		indices.push_back(mesh->mFaces[i].mIndices[0]);
//		indices.push_back(mesh->mFaces[i].mIndices[1]);
//		indices.push_back(mesh->mFaces[i].mIndices[2]);
//	}
//
//
//}

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices, std::vector<ULONG>& indices, std::vector<texture> vTexture)
	: vertices(vertices), vTexture(vTexture), pContext(pContext)
{
	if (FAILED(this->vertexBuffer.Init(pDevice, vertices.data(), vertices.size()/* + 1*/)))
		throw;
	if (FAILED(this->indexBuffer.Init(pDevice, indices.data(), indices.size())))
		throw;
	
}

Mesh::Mesh(const Mesh& mesh)
{
	this->vertices = mesh.vertices;
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
