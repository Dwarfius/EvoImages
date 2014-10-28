#pragma once

#include "GlobalHeader.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "Tree.h"
#include "Camera.h"

class Game : public BaseGame
{
public:
	Game();
	~Game() { Release(); };
	void SetEngine(Engine *engine);
	bool Init();
	virtual void Release();
	virtual void OnCreateDevice(LPDIRECT3DDEVICE9 device);
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 device);
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual void OnUpdateFrame(LPDIRECT3DDEVICE9 device);
	virtual void OnRenderFrame(LPDIRECT3DDEVICE9 device);
	virtual void HandleInput();

private:
	void UpdateImages();

	Engine *engine;
	Camera camera;
	LPDIRECT3DTEXTURE9 mainTexture[3];
	TextRenderer text;
	LPD3DXSPRITE sprite;
	Tree *trees;
	string *treeStrings;
};