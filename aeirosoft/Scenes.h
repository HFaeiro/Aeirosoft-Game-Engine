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
	~Scenes();
	bool Initialize();
	virtual std::optional<Events*> Queue();
	virtual void Update();

	bool CreateScene(const std::wstring& sceneName,Gui* gui = nullptr, bool _guiStart = false, bool hideMouse = false);
	void ResetActiveScene();
	void RemoveActiveKeyGui();
	bool SetActiveScene(const std::wstring& sceneName, bool Initalize = true);
	//bool AddObjectToScene(const std::wstring& sceneName, const std::wstring& modelName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	bool AddEntityToScene(std::wstring sceneName, Entity* E, const DirectX::XMFLOAT3& pos = { 0.f,0.f, 0.f }, const DirectX::XMFLOAT3& rot = { 0.f,0.f, 0.f });
	bool AddOnKeyEventToScene(const std::wstring sceneName, const std::wstring guiName, Gui* gui, bool hideMouse, UCHAR Key);
	//bool AddEntityAiToScene(std::wstring sceneName, Ai* E);

	//bool CreateEntityObject(std::wstring filePath, float scale = 1.f);
	//bool AddItem(Item item);
	//bool AddItemToScene(int itemId, std::wstring sceneName, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);

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
		std::vector<Entity*> entities;
		std::vector<Item*> items;
		bool guiStart;
		bool guiVisible = guiStart;
		bool hideMouse = false;
		Collision* C = new Collision();
		std::unordered_map<UCHAR, std::tuple<Gui*, bool, bool, std::wstring>> OnKeyGui; // UCHAR keypress, bool hidemouse, bool active, std::wstring guiSceneName
		Entity* E = nullptr;
	};

	Scene* ActiveScene = nullptr;
	std::vector<Entity> entities;
	std::vector<Scene> vScenes;
	std::vector<Events*> queued;
	std::vector<Item> items;

	input* i;

};

