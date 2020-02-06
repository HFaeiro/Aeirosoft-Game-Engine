#include "Scenes.h"

bool Scenes::SetActiveScene(const std::wstring& sceneName)
{
	for (auto& s : vScenes)
		if (s.sceneName == sceneName)
		{
			ActiveScene = &s;
			for (auto& m : ActiveScene->vModels)
			{
				C->AddCollidable(&m.m);
			}

			return true;
		}
	return false;
}

bool Scenes::AddModelToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
{
	Model tmpModel;
	for (const auto& m : vModels)
	{
		if (m.modelName == modelName)
		{
			tmpModel = m;
			break;
		}
	}

	if (tmpModel.modelName.empty())
		return false;

	tmpModel.m.setPositionAndRotation(pos, rot);

	for (auto& s : vScenes)
	{
		if (s.sceneName == sceneName)
		{
			s.vModels.push_back(tmpModel);
			return true;
		}
	}

	return false;
}
