#pragma once
#include "framework.h"
#include "Events.h"
#include "model.h"
#include "Gui.h"
#include "helper.h"
#include "Collision.h"
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
			for ( auto& m : ActiveScene->vModels)
			{
				m.m.Render(g->m_TextureShader);
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
	bool AddModelToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
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

	bool AddModel(std::wstring filePath)
	{

		std::wstring modelName = helper::strings::GetNameFromPath(filePath);
		
		for (const auto& m : vModels)
		{
			if (m.modelName == modelName)
				return false;

		}
		
		vModels.push_back(Model(g, modelName, filePath));
		return true;
	}


private:

	//*/input* i;
	graphics* g;
	
	struct Model
	{
		std::wstring modelName;
		model m;
		Model(graphics* g,std::wstring modelName, std::wstring filePath)
		{
			this->modelName = modelName;
			m = model(filePath, g);
		}
		Model() {};

	};
	struct Scene
	{
		std::wstring sceneName;
		//Gui gui;
		std::vector<Model> vModels;
		Player* player;
		Scene(std::wstring n, /*input* i,*/ graphics* g) :sceneName(n)//, gui(g, i)
		{}

	};

	Scene* ActiveScene = nullptr;
	
	std::vector<Model> vModels;
	std::vector<Scene> vScenes;
	Collision* C;
};

