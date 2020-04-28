#pragma once
#include "framework.h"
#include "Events.h"
#include "Entity.h"
#include "tmpEntity.h"
#include "Gui.h"
#include "helper.h"
#include "Player.h"
class Scenes :public Events
{
public:

	Scenes(graphics* g, input* i) : g(g), i(i){}
	~Scenes()
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
		for (auto& ent : entities)
		{
			ent.~EntityObject();
		}
	}
	//bool Initialize()
	//{
	//	return true;
	//}
	bool Initialize()
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
	virtual std::optional<Events*> Queue();
	virtual void Update();

	bool CreateScene(const std::wstring& sceneName,Gui* gui = nullptr, bool _guiStart = false, bool hideMouse = false);
	void ResetActiveEntity();
	void RemoveActiveKeyGui();
	bool SetActiveScene(const std::wstring& sceneName, bool Initalize = true);
	bool AddObjectToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	bool AddEntityToScene(std::wstring sceneName, Entity* E)
	{
		for (auto& s : vScenes)
		{
			if (s.sceneName == sceneName)
			{
				s.E = E;
				s.events.push_back(E);
				s.C->AddCollidable(E);
				return true;
			}
		}
		return false;
	}
	bool AddOnKeyEventToScene(const std::wstring sceneName, const std::wstring guiName, Gui* gui, bool hideMouse, UCHAR Key)
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
	bool AddEntityAiToScene(std::wstring sceneName, EntityAi* E)
	{
		for (auto& s : vScenes)
		{
			if (s.sceneName == sceneName)
			{

				s.events.push_back(E);
				s.C->AddCollidable(E);
				return true;
			}
		}
	}

	bool CreateEntityObject(std::wstring filePath, float scale = 1.f )
	{

		std::wstring entName = helper::strings::GetNameFromPath(filePath);
		
		for (const auto& e : entities)
		{
			if (e.getName() == entName)
				return false;

		}

		entities.emplace_back(EntityObject(g, filePath, scale));
		return true;
	}


private:
	
	graphics* g;
	Timer keyTimer;
	
	struct Scene
	{

		Scene(std::wstring n, graphics* g, bool _guiStart = false) :sceneName(n), guiStart(_guiStart)
		{}
		Scene(std::wstring n, Gui* _gui, graphics* g, bool _guiStart) :sceneName(n), gui(_gui), guiStart(_guiStart)
		{}
		std::wstring sceneName;
		Gui* gui = nullptr;
		std::vector<Events*> events;
		std::vector<EntityObject*> entities;
		bool guiStart;
		bool guiVisible = guiStart;
		bool hideMouse = false;
		Collision* C = new Collision();
		std::unordered_map<UCHAR, std::tuple<Gui*, bool, bool, std::wstring>> OnKeyGui; // UCHAR keypress, bool hidemouse, bool active, std::wstring guiSceneName
		Entity* E = nullptr;
	};

	Scene* ActiveScene = nullptr;
	std::vector<EntityObject> entities;
	std::vector<Scene> vScenes;
	std::vector<Events*> queued;

	
	//window* w;
	input* i;

};

