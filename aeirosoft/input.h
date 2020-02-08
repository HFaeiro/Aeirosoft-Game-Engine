#pragma once
#include "framework.h"
#include <dinput.h>
#include "Events.h"
#include "window.h"
#include "camera.h"
#include "timer.h"
#include "graphics.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
class input : public Events
{
public:

	input(graphics* g, window* w, camera* c, Timer* t) :g(g), w(w), c(c), t(t),
		hInstance(w->getHINST()),
		hWnd(w->getHWND())
	{}
	~input();
	virtual void Update()
	{
		ProcessMouse();
		KeyboardInputs();
		updateTimer();
		
	}

	virtual bool Initialize();

	virtual std::optional<Events*> Queue()
	{
		frame();

		if (w->sizeChange)
		{
			g->Resize();
			w->sizeChange = false;
			
		}

		return this;
	}
	
	
	bool isLeftClick();
	bool isRightClick();
	bool isKey(UCHAR);
	std::wstring fpsString = L"";
	void GetMouse(DirectX::XMFLOAT2& m);
	float GetDeltaTime();
	void GetMouse(POINT& m);
private:
	bool frame();
	int fps = 0;
	Timer deltaTimer;
	void updateTimer()
	{
		fps++;
		if (t->GetMillisecondsElapsed() > 1000.f)
		{
			fpsString = helper::strings::wsinttows(L"FPS: ", fps);
			fps = 0;
			t->restart();
			w->updateTitle(fpsString);
		}
		deltaTimer.restart();
	}
	void ProcessMouse();



	void KeyboardInputs();


	

	

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
	bool AquireMouse();
	bool AquireKeyboard();

	Microsoft::WRL::ComPtr < IDirectInput8 > pDirectInput;
	Microsoft::WRL::ComPtr < IDirectInputDevice8 > pMouse;
	Microsoft::WRL::ComPtr < IDirectInputDevice8 > pKeyboard;

	UCHAR keyState[256];
	DIMOUSESTATE mouseState;
	
	float x, y;
	float mx, my;
	//BYTE    rgbButtons[4];
	HINSTANCE hInstance;
	HWND hWnd;
	graphics* g;
	window* w;
	camera* c;
	Timer* t;
	Timer fsDelta;
};

