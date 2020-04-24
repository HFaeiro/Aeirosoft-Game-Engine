#pragma once
#include "framework.h"
#include "Inc/Assimp/Importer.hpp"
#include <DirectXMath.h>
#include "Timer.h"
#include "Vertex.h"

struct Bone
{
	std::string name;
	DirectX::XMMATRIX offsetMatrix;
	DirectX::XMMATRIX transformationMatrix;
	DirectX::XMMATRIX GlobalTransformationMatrix;
	DirectX::XMMATRIX OGTransformationMatrix;
	DirectX::XMMATRIX finalTransform;
	Bone* parent;
	std::vector<Bone*> vChildren;
	std::vector<Vertex> vertices;
	void TransformBoneGlobals()
	{
		GlobalTransformationMatrix = transformationMatrix;
		Bone* recurBone = parent;
		while (recurBone != nullptr)
		{
			GlobalTransformationMatrix = recurBone->transformationMatrix * GlobalTransformationMatrix;
			recurBone = recurBone->parent;
		}

	}

};


class Animation
{
public:
	Animation() {}
	~Animation() {}
		struct Channel
		{
			struct Key
			{
				float time = 0.f;
				DirectX::XMFLOAT3 pos;
				DirectX::XMMATRIX rot;
				DirectX::XMFLOAT3 scale;
				DirectX::XMMATRIX matrix;
			};
			std::string name;
			std::vector<Key> keys;
		};
		void AddKeyFrameToChannel(std::string channelName, float time, aiVector3D pos, aiQuaternion rot, aiVector3D scale);
		std::vector<DirectX::XMMATRIX> TransformBones(std::vector<Bone*> Bones);
		float TicksPS = 0.f;
		float Duration = 0.f;
		float deltaTime = 0.f;
		bool Active = false;
		Timer deltaTimer;

		std::string name;
		std::vector<Channel> vChannels;

private:
	std::vector<DirectX::XMMATRIX> boneTransforms;
};

