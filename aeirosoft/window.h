#pragma once
#include "framework.h"
#include "Resource.h"
#include <optional>


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

public:
	int getWidth() { return width; }
	int getHeight() { return height; }
	window(HINSTANCE,const std::wstring, POINT);
	HWND& getHWND() {return this->myhWnd;}
	~window();
	static std::optional<int> processMessages();
	
};

