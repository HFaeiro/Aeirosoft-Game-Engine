#include "input.h"

input::input()
{
}

input::~input()
{
	if (pMouse)
		pMouse->Unacquire();
	if (pKeyboard)
		pKeyboard->Unacquire();
}

bool input::initialize(HINSTANCE hInst, HWND hWnd, int screenWidth, int screenHeight)
{
	width = screenWidth;
	height = screenHeight;
	mouseX = 0.f;
	mouseY = 0.f;
	this->hWnd = hWnd;
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, 0)))
		return false;

	if (!AquireMouse())
		return false;
	if (!AquireKeyboard())
		return false;



	return true;
}

bool input::frame()
{
	if (!ReadMouse())
		return false;
	if (!ReadKeyboard())
		return false;
	ProcessInput();
	return true;
}

bool input::isKey(UCHAR keyCode)
{
	return (keyState[keyCode] & 0x80);
	
}

bool input::isLeftClick()
{
	return rgbButtons[0]; 
}

bool input::isRightClick()
{
	return rgbButtons[1];
}

void input::getMouseLocation(float& mX, float& mY)
{
	mX = mouseX;
	mY = mouseY;
}

bool input::ReadKeyboard()
{
	HRESULT hr;
	hr = pKeyboard->GetDeviceState(sizeof(keyState), &keyState);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			pKeyboard->Acquire();
		else
			return false;
	}
	return true;
}

bool input::ReadMouse()
{
	HRESULT hr;
	hr = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	if(FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			pMouse->Acquire();
		else
			return false;
	}
	return true;
}

void input::ProcessInput()
{
	
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;
	*rgbButtons = *mouseState.rgbButtons;

}

bool input::AquireMouse()
{
	if (FAILED(pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, 0)))
		return false;

	if (FAILED(pMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(pMouse->Acquire()))
		return false;

	return true;
}

bool input::AquireKeyboard()
{
	if (FAILED(pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, 0)))
		return false;

	if (FAILED(pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	HRESULT hr = pKeyboard->Acquire();
	if(FAILED(hr))
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			pMouse->Acquire();

	return true;
}
