#pragma once
#include "window.h"
#include "graphics.h"
#include "input.h"
#include "Timer.h"
class app :
	private window
{
	graphics g = graphics(getHWND());
	input pInput;
	Timer timer;

public:
	app(HINSTANCE h, const std::wstring s, POINT p);
	HWND hWnd;
	HINSTANCE hInst;
	~app();
	int begin();


};

