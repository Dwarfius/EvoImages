#include "Input.h"

#pragma warning( disable : 4800 )

LPDIRECTINPUT8 Input::directInput = NULL;
LPDIRECTINPUTDEVICE8 Input::keyboard = NULL;
LPDIRECTINPUTDEVICE8 Input::mouse = NULL;
HWND Input::hWnd = NULL;
char Input::keyboardState[256];
bool Input::pressedKeys[256];
bool Input::prevPressedKeys[256];
DIMOUSESTATE Input::mouseState;
bool Input::pressedBtns[4];
bool Input::prevPressedBtns[4];
POINT Input::mousePos;

void Input::Release()
{
	if(keyboard)
	{
		keyboard->Unacquire();
		SAFE_RELEASE(keyboard);
	}
	if(mouse)
	{
		mouse->Unacquire();
		SAFE_RELEASE(mouse);
	}
	SAFE_RELEASE(directInput);
}

bool Input::Init(HINSTANCE hInstance, HWND hWnd)
{
	ZeroMemory(&keyboardState, sizeof(keyboardState));
	ZeroMemory(&pressedKeys, sizeof(pressedKeys));
	ZeroMemory(&prevPressedKeys, sizeof(prevPressedKeys));
	ZeroMemory(&pressedBtns, sizeof(pressedBtns));
	ZeroMemory(&prevPressedBtns, sizeof(prevPressedBtns));
	ZeroMemory(&mousePos, sizeof(mousePos));

	Release();

	Input::hWnd = hWnd;

	if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
								 (void **)&directInput, NULL)))
	{
		MessageBox(0, "Input Init Failed", "Engine Error", 0);
		return false;
	}

	DWORD coopLevel = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

	//keyboard init
	if(FAILED(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL)) ||
		FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard)) ||
		FAILED(keyboard->SetCooperativeLevel(hWnd, coopLevel)) ||
		FAILED(keyboard->Acquire()))
	{
		MessageBox(0, "Keyboard Init Failed", "Engine Error", 0);
		return false;
	}

	//mouse init
	if(FAILED(directInput->CreateDevice(GUID_SysMouse, &mouse, NULL)) ||
		FAILED(mouse->SetDataFormat(&c_dfDIMouse)) ||
		FAILED(mouse->SetCooperativeLevel(hWnd, coopLevel)) ||
		FAILED(mouse->Acquire()))
	{
		MessageBox(0, "Mouse Init Failed", "Engine Error", 0);
		return false;
	}

	return true;
}

void Input::Update()
{
	if(!keyboard || !mouse)
		return;

	//mouse
	HRESULT hr = mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
	{
		mouse->Acquire();	
		return;
	}
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);
	memcpy(prevPressedBtns, pressedBtns, sizeof(prevPressedBtns));
	for(int i=0; i<4; i++)
		prevPressedBtns[i] = mouseState.rgbButtons[i] & 0x80;

	//keyboard
	hr = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
	{
		keyboard->Acquire();
		return;
	}
	memcpy(prevPressedKeys, pressedKeys, sizeof(prevPressedKeys));
	for(int i=0; i<256; i++)
		pressedKeys[i] = keyboardState[i] & 0x80;
}
