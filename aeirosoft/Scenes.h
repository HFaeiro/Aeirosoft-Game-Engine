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

	Scenes(graphics* g, Collision* C, window * _w) : g(g), C(C), w(_w){}
	~Scenes()
	{
		for (const auto& scene : vScenes)
		{
			if (scene.player != nullptr)
				scene.player->~Player();
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
		ActiveScene->events.clear();
		if (ActiveScene->guiStart) {
			if (ActiveScene->gui)
			{
				if (ActiveScene->gui->Initialize())
					ActiveScene->events.push_back(ActiveScene->gui);
				else
					return false;
			}

		}
		else
			if (ActiveScene->player) {
				w->hideMouse = true;
				if (ActiveScene->player->Initialize())
				{
					C->AddCollidable(ActiveScene->player);
					ActiveScene->events.push_back(ActiveScene->player);
				}
				else
					return false;

				
			}
		return true;

	}
	virtual std::optional<Events*> Queue()
	{
		if (ActiveScene->gui) {
			if (!ActiveScene->guiVisible)
			{

				if (ActiveScene->gui->i->isKey(DIK_ESCAPE))
				{
					ActiveScene->gui->ActivateMenu(L"Pause");
					ActiveScene->events.push_back(ActiveScene->gui);
					w->hideMouse = false;
				}
			}
			else
			{
				if (ActiveScene->sceneName == L"Pause")
				{
					ActiveScene->guiVisible = false;
					w->hideMouse = true;
				}
			}
		}
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

	bool CreateScene(const std::wstring& sceneName, Gui* gui = nullptr, bool _guiStart = false);
	bool SetActiveScene(const std::wstring& sceneName);
	bool AddEntityToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	bool AddPlayerToScene(std::wstring sceneName, Player* player)
	{
		for (auto& s : vScenes)
		{
			if (s.sceneName == sceneName)
			{
				s.player = player;
				return true;
			}
		}
		return false;
	}

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
	Player GetActivePlayer()
	{
		if (!ActiveScene->sceneName.empty())
			return *ActiveScene->player;
	}

private:

	//*/input* i;
	graphics* g;
	
	
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
		Player* player = nullptr;
		bool guiStart;
		bool guiVisible = guiStart;

	};

	Scene* ActiveScene = nullptr;
	std::vector<EntityObject> entities;
	std::vector<Scene> vScenes;
	std::vector<Events*> queued;
	Collision* C;
	window* w;

};

