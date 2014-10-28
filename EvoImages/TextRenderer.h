#pragma once

#include "GlobalHeader.h"

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer() { Release(); };

	void Release();
	bool Init(LPDIRECT3DDEVICE9 device, char *faceName, 
		      int size, bool bold = false, bool italic = false);
	void Print(char *text, int xPos, int yPos, DWORD color, 
		       LPD3DXSPRITE sprite = NULL, int textBoxWidth = 0, 
			   int textBoxHeight = 0, DWORD alignment = DT_LEFT);
	void OnLostDevice();
	void OnResetDevice();

private:
	LPD3DXFONT font;
};

//for alignment use DT_LEFT, DT_CENTER, DT_RIGHT, DT_TOP, DT_BOTTOM and |
//combinations