#include "Scenes.h"

bool Scenes::CreateScene(const std::wstring& sceneName, Gui* gui, bool _guiStart)
{
	if (/*this->i == nullptr || */this->g == nullptr)
		return false;

	for (const auto& scene : vScenes)
	{
		if (scene.sceneName == sceneName)
			return false;
	}
	if (gui)
		vScenes.push_back(Scene(sceneName, gui, g, _guiStart));
	else
		vScenes.push_back(Scene(sceneName, g));
	return true;

}

bool Scenes::SetActiveScene(const std::wstring& sceneName)
{
	C->Clear();
	for (auto& s : vScenes)
		if (s.sceneName == sceneName)
		{
			ActiveScene = &s;
			for (auto& e : ActiveScene->entities)
			{
				C->AddCollidable(&e);
			}

			return this->Initialize();
		}
	return false;
}

bool Scenes::AddEntityToScene(const std::wstring& sceneName, const std::wstring& entName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
{
	EntityObject* entModel = nullptr;
	for (auto& e : entities)
	{
		if (e.getName() == entName)
		{
			entModel = &e;
			break;
		}
	}

	if (entModel == nullptr)
		return false;

	entModel->setPositionAndRotation(pos, rot);
	entModel->TransformBounds(entModel->getWorld());
	for (auto& s : vScenes)
	{
		if (s.sceneName == sceneName)
		{
			s.entities.push_back(*entModel);
			return true;
		}
	}

	return false;
}
