#pragma once
#include "framework.h"
#include "Events.h"
#include "Entity.h"
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
		}
	}

	virtual bool Initialize()
	{

		if (!ActiveScene) return false;
		if (ActiveScene->sceneName.empty())
		{
			if (!vScenes.empty()) {
				SetActiveScene(vScenes[0].sceneName);
			}
			else
				return false;
		}
		ActiveScene->events.push_back(ActiveScene->C);

		i->w->hideMouse = ActiveScene->hideMouse;
		for (const auto& event : ActiveScene->events)
		{
			
			event->Initialize();
		}
		////if (ActiveScene->guiStart) {
		////	if (ActiveScene->gui)
		////	{
		////		if (ActiveScene->gui->Initialize())
		////			ActiveScene->events.push_back(ActiveScene->gui);
		////		else
		////			return false;
		////	}

		////}
		////else
		////	if (ActiveScene->player) {
		////		w->hideMouse = true;
		////		if (ActiveScene->player->Initialize())
		////		{
		////			ActiveScene->events.push_back(ActiveScene->player);
		////		}
		////		else
		////			return false;

		////		
		////	}
		return true;

	}
	virtual std::optional<Events*> Queue()
	{
		
		//if (ActiveScene->gui) {
		//	if (ActiveScene->gui->i->isKey(DIK_ESCAPE) && !bEscape)
		//	{
		//		bEscape = true;
		//		std::wstring ActiveMenu = ActiveScene->gui->GetActiveMenu();
		//		if (ActiveMenu != L"Pause" && ActiveScene->sceneName != L"MainMenu")
		//		{
		//			ActiveScene->gui->ActivateMenu(L"Pause");
		//			ActiveScene->events.push_back(ActiveScene->gui);
		//			w->hideMouse = false;
		//		}
		//		else if (ActiveMenu == L"Pause")
		//		{
		//			ActiveScene->gui->ActivateMenu(L"Main");
		//			ActiveScene->guiVisible = false;
		//			Initialize();
		//		}
		//	}
		//	else if(!ActiveScene->gui->i->isKey(DIK_ESCAPE))
		//		bEscape = false;
		//}
		


		for (const auto& E : ActiveScene->events)
			if (const auto optional = E->Queue())
				queued.push_back(*optional);

		return this;
	}
	virtual void Update()
	{

		g->Begin3DScene();
		for (auto& e : ActiveScene->entities)
		{
			e.Render(g->m_TextureShader);
		}

		for (const auto& Q : queued)
			Q->Update();
		queued.clear();


	}

	bool CreateScene(const std::wstring& sceneName,Gui* gui = nullptr, bool _guiStart = false, bool hideMouse = false);
	bool SetActiveScene(const std::wstring& sceneName);
	bool AddObjectToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	bool AddPlayerToScene(std::wstring sceneName, Player* player)
	{
		for (auto& s : vScenes)
		{
			if (s.sceneName == sceneName)
			{
				s.player = player;
				s.events.push_back(player);
				s.C->AddCollidable(s.player);
				return true;
			}
		}
		return false;
	}
	//bool AddCollidableEventToScene(std::wstring sceneName, void* E)
	//{
	//	for (auto& s : vScenes)
	//	{
	//		if (s.sceneName == sceneName)
	//		{
	//			s.events.push_back((Events*)E);
	//			//s.C->AddCollidable((Collidable*)(E));
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	bool CreateEntityObject(std::wstring filePath)
	{

		std::wstring entName = helper::strings::GetNameFromPath(filePath);
		
		for (const auto& e : entities)
		{
			if (e.getName() == entName)
				return false;

		}
		
		entities.push_back(EntityObject(g, filePath));
		return true;
	}
	//Player GetActivePlayer()
	//{
	//	if (ActiveScene->player)
	//		return *ActiveScene->player;

	//}

private:

	//*/input* i;
	graphics* g;
	bool bEscape = false;
	
	struct Scene
	{

		Scene(std::wstring n, graphics* g, bool _guiStart = false) :sceneName(n), guiStart(_guiStart)
		{}
		Scene(std::wstring n, Gui* _gui, graphics* g, bool _guiStart) :sceneName(n), gui(_gui), guiStart(_guiStart)
		{}
		std::wstring sceneName;
		Gui* gui = nullptr;
		std::vector<Events*> events;
		std::vector<EntityObject> entities;
		bool guiStart;
		bool guiVisible = guiStart;
		bool hideMouse = false;
		Player* player;
		Collision* C = new Collision();

	};

	Scene* ActiveScene = nullptr;
	std::vector<EntityObject> entities;
	std::vector<Scene> vScenes;
	std::vector<Events*> queued;
	//window* w;
	input* i;

};

