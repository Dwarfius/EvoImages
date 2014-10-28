#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

using namespace std;

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
};

//this project uses /MTd flag for linking C/C++ libraries - http://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx