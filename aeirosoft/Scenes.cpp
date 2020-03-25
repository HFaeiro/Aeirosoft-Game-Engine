#include "Scenes.h"

bool Scenes::CreateScene(const std::wstring& sceneName, Gui* gui, bool _guiStart, bool hidemouse)
{
	if (/*this->i == nullptr || */this->g == nullptr)
		return false;

	for (const auto& scene : vScenes)
	{
		if (scene.sceneName == sceneName)
			return false;
	}
	
	Scene tmpScene = Scene(sceneName, gui, g, _guiStart);
	tmpScene.hideMouse = hidemouse;
	//tmpScene.events.push_back(tmpScene.C);
	if (_guiStart)
		tmpScene.events.push_back(gui);
	vScenes.push_back(tmpScene);
	return true;

}

bool Scenes::SetActiveScene(const std::wstring& sceneName)
{
	for (auto& s : vScenes)
		if (s.sceneName == sceneName)
		{
			ActiveScene = &s;
			return this->Initialize();
			return true;
		}
	return false;
}

bool Scenes::AddObjectToScene(const std::wstring& sceneName, const std::wstring& entName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
{
	EntityObject* entModel = nullptr;
	for (const auto& e : entities)
	{
		if (e.getName() == entName)
		{
			entModel = new EntityObject(e);
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
			s.C->AddCollidable(entModel);
			return true;
		}
	}

	return false;
}
