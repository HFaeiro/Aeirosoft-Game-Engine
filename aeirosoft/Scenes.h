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

	Scenes(graphics* g, Collision* C) : g(g), C(C){}
	~Scenes() {}

	virtual bool Initialize()
	{
		if (ActiveScene->sceneName.empty())
		{
			if (!vScenes.empty()) {
				SetActiveScene(vScenes[0].sceneName);
			}
			else
				return false;
		}
		if (!ActiveScene->player->Initialize())
			return false;
		C->AddCollidable(ActiveScene->player);
		return true;

	}
	virtual std::optional<Events*> Queue()
	{
		return this;
	}
	virtual void Update()
	{
		g->Begin3DScene();
			for ( auto& e : ActiveScene->entities)
			{
				e.Render(g->m_TextureShader);
			}
			if (ActiveScene->player->Queue())
				ActiveScene->player->Update();
	}

	bool CreateScene(const std::wstring& sceneName)
	{
		if (/*this->i == nullptr || */this->g == nullptr)
			return false;

		for (const auto& scene : vScenes)
		{
			if (scene.sceneName == sceneName)
				return false;
		}

		vScenes.push_back(Scene(sceneName,/* i,*/ g));
		return true;

	}
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


private:

	//*/input* i;
	graphics* g;
	
	
	struct Scene
	{
		std::wstring sceneName;
		//Gui gui;

		std::vector<EntityObject> entities;
		Player* player;
		Scene(std::wstring n, /*input* i,*/ graphics* g) :sceneName(n)//, gui(g, i)
		{}

	};

	Scene* ActiveScene = nullptr;
	std::vector<EntityObject> entities;
	std::vector<Scene> vScenes;
	Collision* C;
};

