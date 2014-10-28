#include "GlobalHeader.h"
#include "Renderer.h"

#pragma once
class BaseGame
{
public:
	virtual ~BaseGame() {};
	virtual void Release() = NULL;
	virtual void OnCreateDevice(LPDIRECT3DDEVICE9 device) = NULL;
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 device) = NULL;
	virtual void OnLostDevice() = NULL;
	virtual void OnDestroyDevice() = NULL;
	virtual void OnUpdateFrame(LPDIRECT3DDEVICE9 device) = NULL;
	virtual void OnRenderFrame(LPDIRECT3DDEVICE9 device) = NULL;
	virtual void HandleInput() = NULL;
};

class Engine
{
public:
	Engine(BaseGame *game);
	~Engine() {Release();};

	bool Init(char *title, HINSTANCE hInstance, int width, int height, bool windowed = true);
	void Run();
	void Release();
	void ToggleFullscreen();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ToggleWireframe();
	
	DWORD GetFillMode() { return fillMode; };
	int GetWidth();
	int GetHeight();

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnCreateDevice();
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();
	void OnUpdateFrame();
	void OnRenderFrame();

	HWND hWnd;
	HINSTANCE hInstance;
	int windowWidth, windowHeight;
	int fullScreenWidth, fullScreenHeight;
	DWORD fillMode;
	WINDOWPLACEMENT windowPlacement;
	BaseGame *game;
	Renderer *renderer;
	bool active;
};

