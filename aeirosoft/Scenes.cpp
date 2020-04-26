#include "Scenes.h"

bool Scenes::CreateScene(const std::wstring& sceneName, Gui* gui, bool _guiStart, bool hidemouse)
{
	if (/*this->i == nullptr || */this->g == nullptr)
		return false;

	if (sceneName.empty())
		return false;


	for (const auto& scene : vScenes)
	{
		if (scene.sceneName == sceneName)
			return false;
	}
	
	Scene tmpScene = Scene(sceneName, gui, g, _guiStart);
	tmpScene.hideMouse = hidemouse;
	if (_guiStart)
		tmpScene.events.push_back(gui);
	vScenes.push_back(tmpScene);
	return true;

}
void Scenes::ResetActiveEntity()
{
	if (ActiveScene != nullptr)
	{
		if (ActiveScene->E != nullptr)
			ActiveScene->E->Initialize();
	}
}
void Scenes::RemoveActiveKeyGui()
{
	int count = 0;
	if (!ActiveScene->OnKeyGui.empty())
	{
		for (auto& pair : ActiveScene->OnKeyGui)
		{
			for (auto& E : ActiveScene->events)
			{

				if (E == std::get<0>(pair.second))
				{
					ActiveScene->events.erase(ActiveScene->events.begin() + count);
					i->w->hideMouse = !std::get<1>(pair.second);
					std::get<2>(pair.second) = false;
				}
				count++;
			}
			count = 0;
		}
	}
}
bool Scenes::SetActiveScene(const std::wstring& sceneName, bool Initialize)
{
	for (auto& s : vScenes)
		if (s.sceneName == sceneName)
		{
			ActiveScene = &s;
			if(Initialize)
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
			s.entities.push_back(entModel);
			s.C->AddCollidable(entModel);
			return true;
		}
	}

	return false;
}
