#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
	font = NULL;
}

void TextRenderer::Release()
{
	SAFE_RELEASE(font);
}

bool TextRenderer::Init(LPDIRECT3DDEVICE9 device, char *faceName, int size, 
						bool bold, bool italic)
{
	Release();
	HRESULT hr = D3DXCreateFont(device, -size, 0, bold ? 800 : 0, 1, italic, DEFAULT_CHARSET,
		                        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
								faceName, &font);
	if(FAILED(hr))
	{
		MessageBox(0, "Font creation failed", "Engine Error", 0);
		return false;
	}
	return true;
}

int TextRenderer::Print(char *text, int xPos, int yPos, DWORD color, LPD3DXSPRITE sprite,
	                     int textBoxWidth, int textBoxHeight, DWORD alignment)
{
	if(!font)
		return 0;

	DWORD format = DT_EXPANDTABS;
	if(textBoxWidth == 0)
		format |= DT_NOCLIP;
	else
	{
		format |= DT_WORDBREAK;
		format |= alignment;
		if(textBoxHeight == 0)
			textBoxHeight = 2000; //making it big enough to enclose everything
	}
	RECT rect = {xPos, yPos, xPos + textBoxWidth, yPos + textBoxHeight};
	font->DrawText(sprite, text, -1, &rect, format, color);
}

void TextRenderer::OnLostDevice()
{
	if(font)
		font->OnLostDevice();
}

void TextRenderer::OnResetDevice()
{
	if(font)
		font->OnResetDevice();
}