#include "model.h"
#pragma comment(lib, "assimp-vc142-mtd.lib")



static bool _compare_min_x(Vertex const& p1, Vertex const& p2) { return p1.position.x < p2.position.x; }
static  bool _compare_min_y(Vertex const& p1, Vertex const& p2) { return p1.position.y < p2.position.y; }
static bool _compare_min_z(Vertex const& p1, Vertex const& p2) { return p1.position.z < p2.position.z; }

static bool _compare_max_x(Vertex const& p1, Vertex const& p2) { return p1.position.x > p2.position.x; }
static  bool _compare_max_y(Vertex const& p1, Vertex const& p2) { return p1.position.y > p2.position.y; }
static bool _compare_max_z(Vertex const& p1, Vertex const& p2) { return p1.position.z > p2.position.z; }


void model::init(const std::string& filename, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	this->pDevice = pDevice;
	this->pContext = pContext;
	initBuffers(filename);


}

model::model()
{

}

model::~model()
{

}

void model::Render()
{
	
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();
	
	//Box.Draw(pContext.Get(), world);

}



void model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}

}

Mesh model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		if (mesh->mTextureCoords[0])
		{
			vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);

	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<texture> vTexture;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<texture> diffuseTextures = LoadTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);

	CreateBounding(vertices);
	//meshes.push_back(Mesh(pDevice.Get(), pContext.Get(), boundingVerts, boundingIndecies, diffuseTextures));

	return Mesh(pDevice.Get(), pContext.Get(), vertices, indices, diffuseTextures);

}

bool model::LoadModel(const std::string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;
	this->ProcessNode(pScene->mRootNode, pScene);

	return true;
}

std::vector<texture> model::LoadTextures(aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene)
{
	std::vector<texture> vTexture;
	TextureStorageType storeType = TextureStorageType::Invalid;
	UINT textureCount = pMaterial->GetTextureCount(type);
	if (textureCount == NULL)
	{
		storeType = TextureStorageType::None;
		aiColor3D aiColor(0.f, 0.f, 0.f);
		switch (type)
		{
		case aiTextureType_DIFFUSE:
		{
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack())
			{
				vTexture.push_back(texture(this->pDevice.Get(), Colors::UnloadedTextureColor, type));
				return vTexture;
			}
			vTexture.push_back(texture(this->pDevice.Get(), Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), type));
			return vTexture;
		}
		}
	}
	else
	{
		vTexture.push_back(texture(this->pDevice.Get(), Colors::UnhandledTextureColor, type));
		return vTexture;
	}
}


void model::CreateBounding(std::vector<Vertex> v)
{

		DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
		if (v.size() > 1) {
			minVertex.x = (*std::min_element(v.begin(), v.end(), &_compare_min_x)).position.x;
			minVertex.y = (*std::min_element(v.begin(), v.end(), &_compare_min_y)).position.y;
			minVertex.z = (*std::min_element(v.begin(), v.end(), &_compare_min_z)).position.z;
	
			maxVertex.x = (*std::min_element(v.begin(), v.end(), &_compare_max_x)).position.x;
			maxVertex.y = (*std::min_element(v.begin(), v.end(), &_compare_max_y)).position.y;
			maxVertex.z = (*std::min_element(v.begin(), v.end(), &_compare_max_z)).position.z;
	
		}


		bBox.CreateFromPoints(bBox, DirectX::XMVectorSet(minVertex.x, minVertex.y, minVertex.z, 0.f), DirectX::XMVectorSet(maxVertex.x, maxVertex.y, maxVertex.z, 0.f));

		//bBox = DirectX::BoundingBox(minVertex, maxVertex);


}

//void model::CreateBounding(std::vector<Vertex> v)
//{
//	std::vector<Vertex> boundingVerts;
//	std::vector<DWORD> boundingIndecies;
//	DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
//	DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
//
//	if (v.size() > 1) {
//		minVertex.x = (*std::min_element(v.begin(), v.end(), &_compare_min_x)).position.x;
//		minVertex.y = (*std::min_element(v.begin(), v.end(), &_compare_min_y)).position.y;
//		minVertex.z = (*std::min_element(v.begin(), v.end(), &_compare_min_z)).position.z;
//
//		maxVertex.x = (*std::min_element(v.begin(), v.end(), &_compare_max_x)).position.x;
//		maxVertex.y = (*std::min_element(v.begin(), v.end(), &_compare_max_y)).position.y;
//		maxVertex.z = (*std::min_element(v.begin(), v.end(), &_compare_max_z)).position.z;
//
//	}
//	float distX = (maxVertex.x - minVertex.x) / 2.0f;
//	float distY = (maxVertex.y - minVertex.y) / 2.0f;
//	float distZ = (maxVertex.z - minVertex.z) / 2.0f;
//
//	DirectX::XMVECTOR objectCenterOffset = DirectX::XMVectorSet(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);
//
//	// Compute bounding sphere (distance between min and max bounding box vertices)
//	float boundingSphere = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(distX, distY, distZ, 0.0f)));
//
//	// Front Vertices
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(minVertex.x, minVertex.y, minVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(minVertex.x, maxVertex.y, minVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(maxVertex.x, maxVertex.y, minVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(maxVertex.x, minVertex.y, minVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//
//	// Back Vertices
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(minVertex.x, minVertex.y, maxVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(maxVertex.x, minVertex.y, maxVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(maxVertex.x, maxVertex.y, maxVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//	boundingVerts.push_back(Vertex(DirectX::XMFLOAT3(minVertex.x, maxVertex.y, maxVertex.z), DirectX::XMFLOAT2(0.f, 0.f)));
//
//	DWORD* i = new DWORD[36];
//
//	// Front Face
//	i[0] = 0; i[1] = 1; i[2] = 2;
//	i[3] = 0; i[4] = 2; i[5] = 3;
//
//	// Back Face
//	i[6] = 4; i[7] = 5; i[8] = 6;
//	i[9] = 4; i[10] = 6; i[11] = 7;
//
//	// Top Face
//	i[12] = 1; i[13] = 7; i[14] = 6;
//	i[15] = 1; i[16] = 6; i[17] = 2;
//
//	// Bottom Face
//	i[18] = 0; i[19] = 4; i[20] = 5;
//	i[21] = 0; i[22] = 5; i[23] = 3;
//
//	// Left Face
//	i[24] = 4; i[25] = 7; i[26] = 1;
//	i[27] = 4; i[28] = 1; i[29] = 0;
//
//	// Right Face
//	i[30] = 3; i[31] = 2; i[32] = 6;
//	i[33] = 3; i[34] = 6; i[35] = 5;
//
//	for (int j = 0; j < 36; j++)
//		boundingIndecies.push_back(i[j]);
//
//	
//	vBVerts.push_back(boundingVerts);
//	vBIndecies.push_back(boundingIndecies);
//}

void model::FillBoundingBuffers()
{

	//for (int i; i < vBVerts.size(); i++)
	//{
	//	this->vertexBuffer.Init(pDevice.Get(), vBVerts[i].data(), vBVerts[i].size())
	//		
	//}

}



void model::ReleaseTexture()
{
}

bool model::initBuffers(const std::string& filename)
{
	if (!LoadModel(filename))
		return false;


	/*std::vector<Vertex> vvertices;
	std::vector<DWORD> vindices;




	Vertex v[] =
	{
		{-0.5f, -0.5f, 1.f , 0.f, 1.f},
		{-0.5f, 0.5f, 1.f , 0.f, 0.f},
		{0.5f, 0.5f, 1.f , 1.f, 0.f},
		{0.5f, -0.5f, 1.f , 1.f, 1.f}
	};

	for (int i = 0; i < ARRAYSIZE(v); i++)
		vvertices.push_back(v[i]);

	DWORD ind[] =
	{
		0,1,2,
		0,2,3
	};

	
	for (int i = 0; i < ARRAYSIZE(ind); i++)
		vindices.push_back(ind[i]);




	meshes.push_back(Mesh(pDevice.Get(), pContext.Get(), vvertices, vindices));
	*/
	return true;
}
