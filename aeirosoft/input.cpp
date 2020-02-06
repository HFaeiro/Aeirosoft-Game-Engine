#include "input.h"



input::~input()
{
	if (pMouse)
		pMouse->Unacquire();
	if (pKeyboard)
		pKeyboard->Unacquire();
}

bool input::Initialize()
{

	x = 0.f;
	y = 0.f;
	this->hWnd = hWnd;
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, 0)))
		return false;

	AquireMouse();
	AquireKeyboard();
	deltaTimer.Start();
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

void input::ProcessMouse()
{

	float clicksPerSecond = 5;
	float mouseSensitivity = .005f;

	

	//if (x)
	x = mx * mouseSensitivity;
	//if (y)
	y = my * mouseSensitivity;


	if (y < -1.5f)
	{
		y = -1.4f;
		my = -301.f;
	}
	if (y > 1.5f)
	{
		y = 1.4f;
		my = 300.f;
		
	}
	g->SetCamRotation(y, x, 0.f);

}
void input::GetMouse(POINT& m)
{
	m = w->getMousePosition();
}
void input::GetMouse(DirectX::XMFLOAT2& m)
{
	m.x = x;
	m.y = y;
}
float input::GetDeltaTime()
{
	return (deltaTimer.GetMillisecondsElapsed() / 1000);
}
void input::KeyboardInputs()
{
	float dt = (deltaTimer.GetMillisecondsElapsed() / 1000);
	float moveSpeed = 50 * dt;

	if (isKey(DIKEYBOARD_F11))
	{
		double fsdelta = fsDelta.GetSecondsElapsed();
		if (fsdelta >= .5 || fsdelta == 0)
		{
			g->SetFullScreen();
			fsDelta.restart();
		}
	}
	if (isKey(DIK_UP) || isKey(DIK_W))
		c->adjustPosition(camera::movementType::forward, moveSpeed);
	if (isKey(DIK_DOWN) || isKey(DIK_S))
		c->adjustPosition(camera::movementType::backward, moveSpeed);

	if (isKey(DIK_LEFT) || isKey(DIK_A))
	{
		c->adjustPosition(camera::movementType::left, moveSpeed);

	}
	if (isKey(DIK_RIGHT) || isKey(DIK_D))
	{
		c->adjustPosition(camera::movementType::right, moveSpeed);
	}

	//if (isKey(DIK_SPACE))
	//	c->adjustPosition(camera::movementType::up, moveSpeed);

	//if (isKey(DIK_LSHIFT))
	//	c->adjustPosition(camera::movementType::up, -moveSpeed);
	//

}

bool input::isKey(UCHAR keyCode)
{
	return (keyState[keyCode] & 0x80);

}

bool input::isLeftClick()
{
	return w->LeftClick;
}

bool input::isRightClick()
{
	return w->RightClick;
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
	if (FAILED(hr))
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

	mx += mouseState.lX;
	my += mouseState.lY;
	//*rgbButtons = *mouseState.rgbButtons;

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
	if (FAILED(hr))
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			pMouse->Acquire();

	return true;
}
