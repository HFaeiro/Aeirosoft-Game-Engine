#pragma once
#include "framework.h"
#include "Resource.h"
#include <optional>
#include <windowsx.h>
#include "helper.h"
class window
{

	void MyRegisterClass();
	BOOL InitInstance();
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK tWndProc(HWND, UINT, WPARAM, LPARAM);
	HINSTANCE hInst;                                // current instance
	HWND myhWnd;
	std::wstring sWindowName;
	int width;
	int height;
	POINT p;
	bool active;

public:
	bool hideMouse = false;
	POINT getMousePosition() { return p; }

	int getWidth() { return width; }
	int getHeight() { return height; }
	window(HINSTANCE, const std::wstring, POINT);
	window(const window& w)
	{
		if (this != &w) {
			this->hInst = w.hInst;
			this->myhWnd = w.myhWnd;
			this->sWindowName = w.sWindowName;
			this->width = w.width;
			this->height = w.height;
		}
	}


	HINSTANCE& getHINST() { return this->hInst; }
	HWND& getHWND() { return this->myhWnd; }
	~window();
	void updateTitle(std::wstring);
	static std::optional<int> processMessages();
	bool sizeChange = false;
	bool LeftClick = false;
	bool RightClick = false;
	bool MiddleClick = false;
	
};

