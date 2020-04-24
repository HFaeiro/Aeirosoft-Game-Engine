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

Bone* model::copyConstructBoneRecursive(Bone* const& copyBone, Bone* parent)
{
	Bone* retBone = new Bone();
	if (parent == nullptr)
		pBoneMaster = retBone;
	retBone->name = copyBone->name;
	retBone->offsetMatrix = copyBone->offsetMatrix;
	retBone->OGTransformationMatrix = copyBone->OGTransformationMatrix;
	retBone->GlobalTransformationMatrix = copyBone->GlobalTransformationMatrix;
	retBone->transformationMatrix = copyBone->transformationMatrix;
	retBone->TransformBoneGlobals();

	for (const auto& bone : copyBone->vChildren)
	{
		Bone* child = copyConstructBoneRecursive(bone, retBone);
		if (child != nullptr)
		{
			retBone->vChildren.push_back(child);
		}

	}


	vBones.push_back(retBone);
	return retBone;
}

void model::init(const std::wstring& filename, graphics* g, float scale)
{
	
	this->g = g;
	this->scale = scale;
	this->directory = helper::strings::GetDirectoryFromPath(filename);
	this->pDevice = g->GetDevice();
	this->pContext = g->GetDeviceContext();
	initBuffers(filename);
	
}


model::model(const std::wstring& filename, graphics* g, float scale): g(g)
{

	this->scale = scale;
	this->pDevice = g->GetDevice();
	this->pContext = g->GetDeviceContext();
	this->pos = DirectX::XMFLOAT3(0, 0, 0);
	this->rot = DirectX::XMFLOAT3(0, 0, 0);
	this->directory = helper::strings::GetDirectoryFromPath(filename);
	initBuffers(filename);
}

model::model()
{

	this->pos = DirectX::XMFLOAT3(0, 0, 0);
	this->rot = DirectX::XMFLOAT3(0, 0, 0);
	
}

model::~model()
{
	//for (auto& mesh : meshes)
	//{
	//	mesh.~Mesh();
	//}
	//for (auto& bone : vBones)
	//{
	//	delete bone;
	//	bone = nullptr;
	//}
	//for (auto& bone : vBonesTmp)
	//{
	//	if (bone != nullptr)
	//	{
	//		delete bone;
	//		bone = nullptr;
	//	}
	//}
	//meshes.clear();
	//vBones.clear();
}
void model::LookAt(DirectX::XMFLOAT3 f3)
{
	using namespace DirectX;
	XMVECTOR v = { f3.x, f3.y, f3.z, 1.0f };
	v = DirectX::XMVector3Normalize(v);
	float dot = XMVectorGetX(XMVector3Dot(DefaultForward, v));
	if (std::abs(dot - (-1.f)) < 0.000001f)
	{
		return;
	}
	if (std::abs(dot - (1.f)) < 0.000001f)
	{
		return;
	}
	float rotAngle = (float)std::acos(dot);
	XMVECTOR rotAxis = XMVector3Cross(DefaultForward, v);
	rotAxis = XMVector3Normalize(rotAxis);
	XMFLOAT4 rot4;
	XMStoreFloat4(&rot4, rotAxis);

	float halfAngle = rotAngle * .5f;
	float s = (float)std::sin(halfAngle);

	XMFLOAT4 quat = { /*rot4.x * s*/0, rot4.y * s,/* rot4.z * s*/0, (float)std::cos(halfAngle)};
	prevWorld = world;
	world = XMMatrixRotationQuaternion(XMLoadFloat4(&quat)) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	
	return;
}
model::model(const model& m)
{
	*this = m;
	int numBones = vBones.size();
	if (numBones)
	{
		vBones.clear();
		copyConstructBoneRecursive(m.pBoneMaster, nullptr);
	}
}

void model::adjustPosition(DirectX::XMFLOAT3 pos)
{

	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
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
	prevPos = pos;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	UpdateWorldMatrix();

}
void model::setPosition(float x, float y)
{
	prevPos = pos;
	pos.x = x;
	pos.y = y;
	UpdateWorldMatrix();

}
void model::setPositionAndRotation(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot)
{
	prevPos = pos;
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
	prevPos = pos;
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
bool model::isAnimActive()
{
	if (currentAnim != nullptr)
		return currentAnim->Active;
	return false;
}
void model::SetCurrentAnimation(std::string animName)
{

	if (animName == "")
	{
		if (currentAnim != nullptr)
		{
			currentAnim->deltaTimer.Stop();
			currentAnim->deltaTime = 0;
			currentAnim->Active = false;
			currentAnim = nullptr;
		}

		return;
	}
	else
		if (currentAnim != nullptr)
			if (animName == currentAnim->name)
			{
				currentAnim->Active = true;
				return;
			}
	if (vAnimations.size())
	{
		for (auto& anim : vAnimations)
		{
			if (anim.name.find(animName) != std::string::npos)
			{
				
				if(currentAnim == nullptr || currentAnim->name.find(animName) == std::string::npos)
					currentAnim = &anim;
				currentAnim->Active = true;
				return;
			}
		}
	}
	currentAnim = nullptr;
	return;
}
void model::Render(TextureShader pTextureShader)
{
	
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTextureShader.UpdateWorldMatrixBuffer(pContext.Get(), world);

	if(currentAnim != nullptr)
	{

		pTextureShader.SetShaders(pContext.Get(), true);
		pTextureShader.UpdateBonesBuffer(pContext.Get(), currentAnim->TransformBones(vBones));

	}
	else
		pTextureShader.SetShaders(pContext.Get());

	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();

}


bool model::LoadModel(const std::wstring& filename)
{
	Assimp::Importer importer;
	//importer.SetPropertyBool(AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES, false);
	std::string str = helper::strings::wideToChar(filename);
	const aiScene* pScene = importer.ReadFile(str, aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_Fast |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	for (UINT i = 0; i < pScene->mNumMeshes; i++)
	{

		meshes.push_back(this->ProcessMesh(pScene->mMeshes[i], pScene));
	}

	if (pScene->HasAnimations())
	{
		for (int i = 0; i < pScene->mNumAnimations; i++)
		{
			Animation animation;
			animation.name = pScene->mAnimations[i]->mName.data;
			if (animation.name == "")
			{
				animation.name = "Anim" + std::to_string(vAnimations.size());
			}
			animation.TicksPS = pScene->mAnimations[i]->mTicksPerSecond;
			animation.Duration = pScene->mAnimations[i]->mDuration;
			for (int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++)
			{
				const aiNodeAnim* tmpChannel = pScene->mAnimations[i]->mChannels[j];
				std::string name = tmpChannel->mNodeName.data;

				for (int k = 0; k < tmpChannel->mNumPositionKeys; k++)
				{
					animation.AddKeyFrameToChannel(name, tmpChannel->mPositionKeys[k].mTime, tmpChannel->mPositionKeys[k].mValue,
						tmpChannel->mRotationKeys[k].mValue, tmpChannel->mScalingKeys[k].mValue);
				}
			}
			vAnimations.push_back(animation);
		}

		vBones.push_back(pBoneMaster);

	}
	return true;
}

Bone* model::CreateBoneTreeRecursive(aiNode* node, Bone* parent)
{
	Bone* retBone = new Bone();
	if (parent == nullptr)
		pBoneMaster = retBone;
	retBone->parent = parent;
	retBone->name = node->mName.data;
	retBone->transformationMatrix = aiMatrix4x4ToDXMatrix(node->mTransformation);
	retBone->OGTransformationMatrix = retBone->transformationMatrix;
	retBone->TransformBoneGlobals();

	for (int i = 0; i < node->mNumChildren; i++) {
		Bone* child = CreateBoneTreeRecursive(node->mChildren[i], retBone);
		if (child != nullptr)
		{
			retBone->vChildren.push_back(child);
		}

	}


	vBonesTmp.push_back(retBone);
	return retBone;

}


DirectX::XMMATRIX model::aiMatrix4x4ToDXMatrix(aiMatrix4x4 in)
{
	//this is a reason to dislike assimp ;
	//but i mean it made it more portable
	DirectX::XMFLOAT4X4 tmp;
	tmp._11 = in.a1; tmp._12 = in.a2; tmp._13 = in.a3; tmp._14 = in.a4;
	tmp._21 = in.b1; tmp._22 = in.b2; tmp._23 = in.b3; tmp._24 = in.b4;
	tmp._31 = in.c1; tmp._32 = in.c2; tmp._33 = in.c3; tmp._34 = in.c4;
	tmp._41 = in.d1; tmp._42 = in.d2; tmp._43 = in.d3; tmp._44 = in.d4;
	
	return DirectX::XMLoadFloat4x4(&tmp);

}

void model::UpdateWorldMatrixWithViewMatrix(DirectX::XMMATRIX viewMatrix)
{
	prevWorld = world;
	world = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * viewMatrix;

}

void model::UpdateWorldMatrix()
{
	prevWorld = world;
	world = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//TransformBounds(world);



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


	

		if (mesh->mTextureCoords[0] && mesh->mNormals)
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.emplace_back(mesh->mVertices[i].x * scale,
					mesh->mVertices[i].y * scale,
					mesh->mVertices[i].z * scale,
					(float)mesh->mTextureCoords[0][i].x,
					(float)mesh->mTextureCoords[0][i].y,
					(float)mesh->mNormals[i].x,
					(float)mesh->mNormals[i].y,
					(float)mesh->mNormals[i].z
				);
			}
		else if (mesh->mTextureCoords[0])
		{
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.emplace_back(mesh->mVertices[i].x * scale,
					mesh->mVertices[i].y * scale,
					mesh->mVertices[i].z * scale,
					(float)mesh->mTextureCoords[0][i].x,
					(float)mesh->mTextureCoords[0][i].y
					);
			}
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

	if (mesh->HasBones())
	{		
		int boneNum = 0;
		int offset = 0;
		if(!vBones.size())
			for(int i = 0; i < scene->mRootNode->mNumChildren; i++)
				CreateBoneTreeRecursive(scene->mRootNode->mChildren[i], nullptr);
		for (int i = 0; i < mesh->mNumBones; i++)
		{

			for (auto& bone : vBonesTmp)
			{
				
				if (bone->name == mesh->mBones[i]->mName.data) {
					bone->offsetMatrix = aiMatrix4x4ToDXMatrix(mesh->mBones[i]->mOffsetMatrix);
					vBones.push_back(bone);
					vBonesTmp.erase(vBonesTmp.begin() + boneNum);
					break;
				}
				else
					boneNum++;
			}
			offset = vBones.size();
			for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				if (mesh->mBones[i]->mWeights[j].mVertexId < vertices.size())
				{
					vertices[mesh->mBones[i]->mWeights[j].mVertexId].AddWeights(i, mesh->mBones[i]->mWeights[j].mWeight);
					if(vBones.size())
						vBones[vBones.size()- 1]->vertices.push_back(vertices[mesh->mBones[i]->mWeights[j].mVertexId]);
				}
			}
			boneNum = 0;
		}
		

	}
	
	return Mesh(pDevice.Get(), pContext.Get(), vertices, indices, diffuseTextures);

}


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
			TextureStorageType storetype = GetStorageType(pScene, pMaterial, i, type);
			
			switch (storetype)
			{
			case TextureStorageType::Disk:
			{
				aiString path;
				pMaterial->GetTexture(type, i, &path);
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

bool model::initBuffers(const std::wstring& filename)
{
	if (!LoadModel(filename))
		return false;

	return true;
}
