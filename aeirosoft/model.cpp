#include "model.h"
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif

static bool _compare_min_x(Vertex const& p1, Vertex const& p2) { return p1.position.x < p2.position.x; }
static  bool _compare_min_y(Vertex const& p1, Vertex const& p2) { return p1.position.y < p2.position.y; }
static bool _compare_min_z(Vertex const& p1, Vertex const& p2) { return p1.position.z < p2.position.z; }

static bool _compare_max_x(Vertex const& p1, Vertex const& p2) { return p1.position.x > p2.position.x; }
static  bool _compare_max_y(Vertex const& p1, Vertex const& p2) { return p1.position.y > p2.position.y; }
static bool _compare_max_z(Vertex const& p1, Vertex const& p2) { return p1.position.z > p2.position.z; }





void model::init(const std::wstring& filename, graphics* g) 
{
	*((Collidable*)this) = Collidable(g);

	this->directory = helper::strings::GetDirectoryFromPath(filename);
	this->pDevice = g->GetDevice();
	this->pContext = g->GetDeviceContext();
	initBuffers(filename);
	
}


model::model(const std::wstring& filename, graphics* g): Collidable(g)
{
	this->pDevice = g->GetDevice();
	this->pContext = g->GetDeviceContext();
	this->scale = DirectX::XMMatrixScaling(1, 1, 1);
	this->pos = DirectX::XMFLOAT3(0, 0, 0);
	this->rot = DirectX::XMFLOAT3(0, 0, 0);
	UpdateWorldMatrix();
	this->directory = helper::strings::GetDirectoryFromPath(filename);
	initBuffers(filename);
}

model::model() : Collidable(nullptr)
{
	this->scale = DirectX::XMMatrixScaling(1, 1, 1);
	this->pos = DirectX::XMFLOAT3(0, 0, 0);
	this->rot = DirectX::XMFLOAT3(0, 0, 0);
	UpdateWorldMatrix();
}

model::~model()
{

}

void model::adjustPosition(DirectX::XMFLOAT3 pos)
{

	this->pos = pos;
	UpdateWorldMatrix();

}
void model::adjustPosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	UpdateWorldMatrix();

}
void model::adjustRotation(float x, float y, float z)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	UpdateWorldMatrix();

}
void model::setPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	UpdateWorldMatrix();

}
void model::setPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
	UpdateWorldMatrix();

}
void model::setPositionAndRotation(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;
	this->rot.x = rot.x;
	this->rot.y = rot.y;
	this->rot.z = rot.z;
	UpdateWorldMatrix();
}
void model::setPosition(DirectX::XMFLOAT3 pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;
	UpdateWorldMatrix();

}
void model::setRotation(DirectX::XMFLOAT3 rot)
{
	this->rot.x = rot.x;
	this->rot.y = rot.y;
	this->rot.z = rot.z;
	UpdateWorldMatrix();

}
void model::setRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
	UpdateWorldMatrix();
}
void model::setRotation(float x, float y)
{
	rot.x = x;
	rot.y = y;
	rot.z = 0;
	UpdateWorldMatrix();
}


void model::Render(TextureShader pTextureShader)
{

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTextureShader.UpdateWorldMatrixBuffer(pContext.Get(), world);

	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();

	//DrawBoundingBox();

}
void model::UpdateScale(DirectX::XMFLOAT3 scale)
{
	this->scale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

void model::UpdateWorldMatrixWithViewMatrix(DirectX::XMMATRIX viewMatrix)
{

	world = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * viewMatrix * scale;

	TransformBounds(world);

}

void model::UpdateWorldMatrix()
{

	world = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * scale;

	TransformBounds(world);



}
bool model::LoadModel(const std::wstring& filename)
{
	Assimp::Importer importer;

	std::string str = helper::strings::wideToChar(filename);
	const aiScene* pScene = importer.ReadFile(str, aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;


	for (UINT i = 0; i < pScene->mNumMeshes; i++)
	{

		meshes.push_back(this->ProcessMesh(pScene->mMeshes[i], pScene));
	}


	return true;
}


void model::ProcessNode(aiScene* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{

		meshes.push_back(this->ProcessMesh(scene->mMeshes[i], scene));
	}

}

Mesh model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	

		if (mesh->mTextureCoords[0])
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.emplace_back(mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z,
					(float)mesh->mTextureCoords[0][i].x,
					(float)mesh->mTextureCoords[0][i].y
				);
			}
		else
		{
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.push_back(Vertex(mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z));
			}
		}
	

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	std::vector<texture> vTexture;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<texture> diffuseTextures = LoadTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);


	CreateBoundingBox(vertices);

	return Mesh(pDevice.Get(), pContext.Get(), vertices, indices, diffuseTextures);

}
//


std::vector<texture> model::LoadTextures(aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene)
{
	std::vector<texture> vTexture;
	TextureStorageType storeType = TextureStorageType::Invalid;
	UINT textureCount = pMaterial->GetTextureCount(type);
	if (textureCount == NULL)
	{
		storeType = TextureStorageType::None;
		aiColor3D aicolor(0.f, 0.f, 0.f);
		switch (type)
		{
		case aiTextureType_DIFFUSE:
		{
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aicolor);
			if (aicolor.IsBlack())
			{
				vTexture.push_back(texture(this->pDevice.Get(), colors::UnloadedTexturecolor, type));
				return vTexture;
			}
			vTexture.push_back(texture(this->pDevice.Get(), color(aicolor.r * 255, aicolor.g * 255, aicolor.b * 255), type));
			return vTexture;
		}
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(type, i, &path);
			TextureStorageType storetype = GetStorageType(pScene, pMaterial, i, type);
			switch (storetype)
			{
			case TextureStorageType::Disk:
			{

				std::wstring filename = this->directory + L'\\' + helper::strings::charToWide(path.C_Str());
				texture diskTexture(pDevice.Get(), filename, type);
				vTexture.push_back(diskTexture);
				break;
			}
			}
		}
	}
	if (!vTexture.size())
		vTexture.push_back(texture(this->pDevice.Get(), colors::UnhandledTexturecolor, type));



	return vTexture;

}

TextureStorageType model::GetStorageType(const aiScene* pScene, aiMaterial* pMat, int index, aiTextureType type)
{
	if (pMat->GetTextureCount(type) == 0)
		return TextureStorageType::None;

	aiString path;
	pMat->GetTexture(type, index, &path);
	std::string texturePath = path.C_Str();
	//Check if texture is an embedded indexed texture by seeing if the file path is an index #
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	//Lastly check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; // No texture exists



}






//void model::FillBoundingBuffers()
//{
//
//	for (int i; i < vBVerts.size(); i++)
//	{
//		this->vertexBuffer.Init(pDevice.Get(), vBVerts[i].data(), vBVerts[i].size())
//			
//	}
//
//}



void model::ReleaseTexture()
{
}

bool model::initBuffers(const std::wstring& filename)
{
	if (!LoadModel(filename))
		return false;

	return true;
}
