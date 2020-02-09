#pragma once
#include "framework.h"
#include "Inc/Assimp/Importer.hpp"
#include "Inc/Assimp/postprocess.h"
#include "Inc/Assimp/scene.h"
#include <algorithm>
#include "graphics.h"
#include "Mesh.h"
#include "TextureShader.h"

class model
{
public:


	model( const std::wstring& filename, graphics *g, float scale = 1.f);
	model();
	~model();

	void init(const std::wstring& filename, graphics *g, float scale = 1.f);

	virtual void adjustPosition(DirectX::XMFLOAT3 pos);
	virtual void adjustPosition(float x, float y, float z);
	virtual void adjustRotation(float x, float y, float z);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(float x, float y);
	void setPositionAndRotation(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot);
	virtual void setPosition(DirectX::XMFLOAT3 pos);
	virtual void setRotation(DirectX::XMFLOAT3 rot);
	virtual void setRotation(float x, float y, float z);
	virtual void setRotation(float x, float y);
	DirectX::XMFLOAT3 getPosition() { return pos; }
	DirectX::XMFLOAT3 getRotation() { return rot; }
	DirectX::XMMATRIX getWorld() { return world; }
	void revertWorld() { world = prevWorld; }
	void Render(TextureShader);
	std::vector<std::vector<Vertex>> getVertices()
	{
		std::vector<std::vector<Vertex>> vertices;
		for (const auto& m : meshes)
		{
			vertices.push_back(m.getVertices());
		}
		return vertices;
	}

	void UpdateWorldMatrixWithViewMatrix(DirectX::XMMATRIX viewMatrix);




	bool active = false;
private:
	bool DEBUG = true;
	void UpdateWorldMatrix();
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX prevWorld;
	Microsoft::WRL::ComPtr < ID3D11Device			> pDevice = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext	> pContext = nullptr;
	
	std::vector<Mesh> meshes;
	void ProcessNode(aiScene* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool LoadModel(const std::wstring& filename);
	std::vector<texture> LoadTextures(aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene);
	TextureStorageType GetStorageType(const aiScene*, aiMaterial*, int index, aiTextureType);
	
	std::vector<std::vector<Vertex>> vBVerts;
	std::vector<std::vector<DWORD>> vBIndecies;
	std::wstring directory = L"";
	
	void ReleaseTexture();
	bool initBuffers(const std::wstring& filename);
	float scale;
	graphics* g;
	


};

