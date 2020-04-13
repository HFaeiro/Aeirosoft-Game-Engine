#pragma once
#include "framework.h"
#include "Inc/Assimp/Importer.hpp"
#include <DirectXMath.h>
#include "Timer.h"
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
		float TicksPS = 0.f;
		float Duration = 0.f;
		std::string name;
		std::vector<Channel> vChannels;
		float deltaTime = 0.f;
		Timer deltaTimer;

};

