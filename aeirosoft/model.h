#pragma once
#include "framework.h"
#include <wrl.h>
#include "Mesh.h"
#include "Inc/Assimp/Importer.hpp"
#include "Inc/Assimp/postprocess.h"
#include "Inc/Assimp/scene.h"
#include <DirectXCollision.h>


class model
{
public:
	
	model();
	~model();
	void init(const std::string & filename, ID3D11Device*, ID3D11DeviceContext*);
	void Render();
	
	std::vector<std::vector<Vertex>> GetBounds() { return vBVerts; }
	DirectX::BoundingBox bBox;
private:
	Microsoft::WRL::ComPtr < ID3D11Device			> pDevice = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext	> pContext = nullptr;

	std::vector<Mesh> meshes;
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool LoadModel(const std::string& filename);
	std::vector<texture> LoadTextures(aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene);

	void CreateBounding(std::vector<Vertex>);
	void DrawBoundings();
	void FillBoundingBuffers();
	std::vector<std::vector<Vertex>> vBVerts;
	std::vector<std::vector<DWORD>> vBIndecies;

	
	void ReleaseTexture();
	bool initBuffers(const std::string & filename);
	
};

