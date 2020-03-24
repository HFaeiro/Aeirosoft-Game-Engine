#pragma once
#include "framework.h"
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "helper.h"
#include "Timer.h"
#include "Gui.h"
#include "Scenes.h"
#include "MovingAimBox.h"
class app :
	public window
{

	Timer m_Timer, m_ClickTimer, m_KeyTimer;

	
	//std::vector<model> vAsteroids;	
	//DirectX::XMVECTOR GetMouseWorldDestination(bool storeOriginAndDestination);
	//void CreateClickRay();


	HWND m_Window;
	std::vector<DirectX::XMVECTOR> vOrig;
	std::vector<DirectX::XMVECTOR> vDest;
	DirectX::XMVECTOR clickOrigin;
	DirectX::XMVECTOR clickDestination;


	std::wstring mousePos;
	std::wstring intersecting = L"Intersecting: FALSE";
	void CreateScenes(Scenes* s, Gui* gui = nullptr);
	void StartupGui(Gui*);
	
	std::vector<int> GetStartInfo(std::string, std::vector<std::string>, std::vector<int> defaults);
	std::vector<Events*> events;
	std::vector<MovingAimBox*> vBoxes;
	Collision* C = nullptr;
	input *i;
	Scenes* s = nullptr;
	Gui* gui = nullptr;
	Player* player = nullptr;
	graphics* m_Graphics;
	int Boxes = NULL;
	bool SetupApplication();
public:
	bool restart = false;
	
	app(HINSTANCE h, const std::wstring s, POINT p);
	HWND hWnd;
	HINSTANCE hInst;
	~app();
	int begin();
	void CheckBounds(model&);



};

