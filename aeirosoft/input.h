#pragma once
#include "framework.h"
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
class input
{
public:
	input();
	~input();

	bool initialize(HINSTANCE, HWND, int, int);
	bool frame();
	bool isKey(UCHAR);
	bool isLeftClick();
	bool isRightClick();
	void getMouseLocation(float&, float&);
private:

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
	int width, height;

	float mouseX, mouseY;
	BYTE    rgbButtons[4];
	HWND hWnd;
};

