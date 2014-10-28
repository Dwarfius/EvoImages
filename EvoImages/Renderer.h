#pragma once

#include "GlobalHeader.h"

class Renderer
{
public:
	//constructors
	Renderer();
	~Renderer() { Release(); };

	//methods
	bool Init(HWND hWnd, bool windowed);
	HRESULT Reset();
	void Release();

	//getters
	LPDIRECT3D9 GetD3D() { return d3d; };
	LPDIRECT3DDEVICE9 GetDevice() { return d3ddev; };
	D3DDISPLAYMODE* GetDisplayMode() { return &d3ddm; };

	//vars
	bool Windowed;

private:
	bool CreatePresParams();

	HWND hWnd;
	HINSTANCE hInstance;
	LPDIRECT3D9 d3d; //controls all rendering
	LPDIRECT3DDEVICE9 d3ddev; //gives access to graphics card
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	D3DCAPS9 d3dcaps;
};