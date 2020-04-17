#include "Animation.h"

void Animation::AddKeyFrameToChannel(std::string channelName, float time, aiVector3D pos, aiQuaternion rot, aiVector3D scale)
{

	DirectX::XMFLOAT4 f4Rot;
	f4Rot.x = rot.x;
	f4Rot.y = rot.y;
	f4Rot.z = rot.z;
	f4Rot.w = rot.w;

	DirectX::XMMATRIX tmpRotQuaternion = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&f4Rot));
	DirectX::XMFLOAT3 framePos = { pos.x, pos.y, pos.z };
	DirectX::XMFLOAT3 frameScale = { scale.x, scale.y, scale.z };

	DirectX::XMMATRIX keyMatrix = DirectX::XMMatrixScaling(frameScale.x, frameScale.y, frameScale.z)
		* tmpRotQuaternion * ( DirectX::XMMatrixTranslation(framePos.x, framePos.y, framePos.z));

	keyMatrix = DirectX::XMMatrixTranspose(keyMatrix);

	Channel::Key key({ time, framePos,tmpRotQuaternion, frameScale, keyMatrix });


	for (auto& channel : vChannels)
	{
		if (channel.name == channelName) {
			channel.keys.push_back(key);
			return;
		}
	}
	Channel newChannel;
	newChannel.name = channelName;
	newChannel.keys.push_back(key);
	vChannels.push_back(newChannel);

}
