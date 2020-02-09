#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "texture.h"
#include "Inc/Assimp/mesh.h"
class Mesh
{
public:
	Mesh(aiMesh*);
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices, std::vector<ULONG>& indices, std::vector<texture>vTexture);
	Mesh(const Mesh& mesh);
	void Draw();
	std::vector<Vertex> getVertices()const{ return vertices; }
private:
	std::wstring name;
	std::vector<Vertex> vertices;
	//std::vector<DWORD> indices;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	std::vector<texture> vTexture;
};


