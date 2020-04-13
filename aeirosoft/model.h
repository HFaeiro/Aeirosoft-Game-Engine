#pragma once
#include "framework.h"
#include "Inc/Assimp/Importer.hpp"
#include "Inc/Assimp/postprocess.h"
#include "Inc/Assimp/scene.h"
#include <algorithm>
#include "graphics.h"
#include "Mesh.h"
#include "TextureShader.h"
#include "Timer.h"
#include "Animation.h"

class model
{
public:

	enum class movementType { forward, backward, up, left, right };
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
	

	bool initBuffers(const std::wstring& filename);
	float scale;
	graphics* g;
	
	DirectX::XMVECTOR vPos;
	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultBackward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR DefaultLeft = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultUp = DirectX::XMVectorSet(0.0f, 1.0f, .0f, 0.0f);

	DirectX::XMVECTOR camForward;
	DirectX::XMVECTOR camBackward;
	DirectX::XMVECTOR camLeft;
	DirectX::XMVECTOR camRight;
	DirectX::XMVECTOR camUp;
	DirectX::XMMATRIX camProjection;




	
	std::vector<Animation> vAnimations;
	std::vector<DirectX::XMMATRIX> boneTransforms;

	struct Bone
	{
		std::string name;
		DirectX::XMMATRIX offsetMatrix;
		DirectX::XMMATRIX transformationMatrix;
		DirectX::XMMATRIX GlobalTransformationMatrix;
		DirectX::XMMATRIX OGTransformationMatrix;
		Bone* parent;
		std::vector<Bone*> vChildren;
	};
	std::vector<Bone*> vBonesTmp;
	std::vector<Bone*> vBones;
	Bone* Bones;
	Bone* CreateBoneTreeRecursive(aiNode*, model::Bone* parent);

	void TransformBoneGlobals(model::Bone* child);

	DirectX::XMMATRIX aiMatrix4x4ToDXMatrix(aiMatrix4x4 in);

};

