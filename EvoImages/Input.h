#pragma once

#include "GlobalHeader.h"

class Input
{
public:
	~Input() { Release(); };
	static void Release();
	static bool Init(HINSTANCE hInstance, HWND hWnd);
	static void Update();

	static POINT GetMousePos() { return mousePos; };
	static long GetXDelta() { return mouseState.lX; };
	static long GetYDelta() { return mouseState.lY; };
	static long GetZDelta() { return mouseState.lZ; }; //mouse wheel
	static bool GetKey(byte key) { return pressedKeys[key]; };
	static bool GetKeyDown(byte key) { return pressedKeys[key] && !prevPressedKeys[key]; };
	static bool GetKeyUp(byte key) { return !pressedKeys[key] && prevPressedKeys[key]; };
	static bool GetButton(byte btn) { return pressedBtns[btn]; };
	static bool GetButtonDown(byte btn) { return pressedBtns[btn] && !prevPressedKeys[btn]; };
	static bool GetButtonUp(byte btn) { return !pressedBtns[btn] && prevPressedBtns[btn]; };

private:
	static LPDIRECTINPUT8 directInput;
	static LPDIRECTINPUTDEVICE8 keyboard, mouse;
	static HWND hWnd;

	//keyboard
	static char keyboardState[256];
	static bool pressedKeys[256];
	static bool prevPressedKeys[256];

	//mouse
	static DIMOUSESTATE mouseState;
	static bool pressedBtns[4];
	static bool prevPressedBtns[4];
	static POINT mousePos;
};

