#include "Scenes.h"


//bool Initialize()
//{
//	return true;
//}

Scenes::~Scenes()
{
	for (const auto& scene : vScenes)
	{
		for (const auto& events : scene.events)
		{
			events->~Events();
		}
		for (const auto& ent : scene.entities)
		{
			delete ent;
		}
		delete scene.C;
	}
	//for (auto& ent : entities)
	//{
	//	ent.~EntityObject();
	//}
}

bool Scenes::Initialize()
{
	if (!ActiveScene)
	{
		if (!vScenes.empty()) {
			SetActiveScene(vScenes[0].sceneName, false);
		}
		else
			return false;
	}

	i->w->hideMouse = ActiveScene->hideMouse;
	for (const auto& event : ActiveScene->events)
	{

		event->Initialize();
	}
	if (ActiveScene->C != nullptr)
		ActiveScene->C->Initialize();
	return true;

}

std::optional<Events*> Scenes::Queue()
{
	
	if (!ActiveScene->OnKeyGui.empty())
	{
		for (auto& pair : ActiveScene->OnKeyGui)
		{
			double kTimer = keyTimer.GetMillisecondsElapsed() * .01;
			if (!kTimer || kTimer > 2)
				if (i->isKey(pair.first))
				{
					keyTimer.restart();
					auto& tuple = pair.second;
					Gui* gui = std::get<0>(tuple);
					if (!std::get<2>(tuple))
					{
						gui->ActivateMenu(std::get<3>(tuple));
						ActiveScene->events.push_back(gui);
						i->w->hideMouse = std::get<1>(tuple);
						std::get<2>(tuple) = true;
					}
					else
					{
						int count = 0;
						for (auto& E : ActiveScene->events)
						{

							if (E == gui)
							{
								ActiveScene->events.erase(ActiveScene->events.begin() + count);
							}
							count++;
						}
						std::get<2>(tuple) = false;
						i->w->hideMouse = !std::get<1>(tuple);
					}
				}
		}
	}


	for (const auto& E : ActiveScene->events)
		if (const auto optional = E->Queue())
			queued.push_back(*optional);
		if (ActiveScene->C->staticCollidable.size())
			ActiveScene->C->Queue();

	return this;
}

void Scenes::Update()
{
	//g->Begin3DScene();
	for (const auto& Q : queued)
		Q->Update();

	queued.clear();
}

bool Scenes::CreateScene(const std::wstring& sceneName, Gui* gui, bool _guiStart, bool hidemouse)
{
	if (this->g == nullptr)
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
void Scenes::ResetActiveScene()
{
	if (ActiveScene != nullptr)
	{
		for(const auto& Entity : ActiveScene->events)
			if (Entity != nullptr)
				Entity->Initialize();
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
			if (Initialize) {
				return this->Initialize();

			}
			return true;
		}
	return false;
}

//bool Scenes::AddObjectToScene(const std::wstring& sceneName, const std::wstring& entName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
//{
//	EntityObject* entModel = nullptr;
//	for (const auto& e : entities)
//	{
//		if (e.getName() == entName)
//		{
//			entModel = new EntityObject(e);
//			break;
//		}
//	}
//
//	if (entModel == nullptr)
//		return false;
//
//	entModel->setPositionAndRotation(pos, rot);
//	entModel->TransformBounds(entModel->getWorld());
//	for (auto& s : vScenes)
//	{
//		if (s.sceneName == sceneName)
//		{
//			s.entities.push_back(entModel);
//			s.C->AddCollidable(entModel);
//			return true;
//		}
//	}
//
//	return false;
//}

bool Scenes::AddEntityToScene(std::wstring sceneName, Entity* E, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
{

	E->setPositionAndRotation(pos, rot);
	E->TransformBounds(E->getWorld());
	for (auto& s : vScenes)
	{
		if (s.sceneName == sceneName)
		{
			s.entities.push_back(E);
			s.events.emplace_back(E);
			s.C->AddCollidable(E);

			return true;
		}
	}
	return false;
}

bool Scenes::AddOnKeyEventToScene(const std::wstring sceneName, const std::wstring guiName, Gui* gui, bool hideMouse, UCHAR Key)
{

	if (sceneName.empty() || guiName.empty() || gui == nullptr || Key == NULL)
		return false;

	for (auto& s : vScenes)
	{
		if (s.sceneName == sceneName)
		{

			if (s.OnKeyGui.emplace(Key, std::make_tuple(gui, hideMouse, false, guiName)).second)
				return true;
			else
				return false;
		}
		else
			return false;
	}

}

//bool Scenes::AddEntityAiToScene(std::wstring sceneName, Entity* E)
//{
//	for (auto& s : vScenes)
//	{
//		if (s.sceneName == sceneName)
//		{
//
//			s.events.emplace_back(E);
//			s.C->AddCollidable(E);
//			return true;
//		}
//	}
//}

//bool Scenes::CreateEntityObject(std::wstring filePath, float scale)
//{
//
//	std::wstring entName = helper::strings::GetNameFromPath(filePath);
//
//	for (const auto& e : entities)
//	{
//		if (e.getName() == entName)
//			return false;
//
//	}
//
//	entities.emplace_back(EntityObject(g, filePath, scale));
//	return true;
//}

//bool Scenes::AddItem(Item item)
//{
//
//	int itemId = item.id;
//
//	for (const auto& i : items)
//	{
//		if (i.id == itemId)
//			return false;
//
//	}
//
//	items.emplace_back(item);
//	return true;
//}

//bool Scenes::AddItemToScene(int itemId, std::wstring sceneName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
//{
//	Item* item = nullptr;
//	for (const auto& i : items)
//	{
//		if (i.id == itemId)
//			item = new Item(i);
//
//	}
//	if (item == nullptr)
//		return false;
//
//	item->setPositionAndRotation(pos, rot);
//	for (auto& s : vScenes)
//	{
//		if (s.sceneName == sceneName)
//		{
//			s.items.push_back(item);
//			s.C->AddCollidable(item);
//			return true;
//		}
//	}
//
//	return false;
//}
