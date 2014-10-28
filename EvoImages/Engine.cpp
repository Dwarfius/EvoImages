#include "Engine.h"
#include "Input.h"
#include "Time.h"

Engine::Engine(BaseGame *game)
{
	this->game = game;
	this->active = true;
	this->renderer = new Renderer();
	fillMode = D3DFILL_SOLID;
}

void Engine::Release()
{
	SAFE_RELEASE(renderer);
	OnLostDevice();
	OnDestroyDevice();
}

bool Engine::Init(char *title, HINSTANCE hInstance, int width, int height, bool windowed)
{
	this->hInstance = hInstance;
	windowWidth = width;
	windowHeight = height;

	//defining the class window
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)Engine::StaticWndProc; //using static to avoit this in argument
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = title;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//registering the class
	RegisterClassEx(&wcex);

	//creating
	hWnd = CreateWindow(title, title,  windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST, 
		CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, this); 

	//adjusting window size
	RECT rect = {0,0, width, height};
	AdjustWindowRect(&rect, GetWindowLong(hWnd, GWL_STYLE), false);
	SetWindowPos(hWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 
				 SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	ZeroMemory(&windowPlacement, sizeof(WINDOWPLACEMENT));
    windowPlacement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWnd, &windowPlacement);

	//render init
	if(!renderer->Init(hWnd, windowed))
		return false;

	fullScreenWidth = renderer->GetDisplayMode()->Width;
	fullScreenHeight = renderer->GetDisplayMode()->Height;

	OnCreateDevice();
	OnResetDevice();
	//========

	//input init
	if(!Input::Init(hInstance, hWnd))
		return false;
	//========
	return true;
}

void Engine::Run()
{
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(active && renderer->GetDevice())
			{
				OnUpdateFrame();
				OnRenderFrame();
			}
		}
	}
}

//Called after D3DDevice is created. Create D3DPOOL_MANAGED resources here.
void Engine::OnCreateDevice()
{
	if(game && renderer)
		game->OnCreateDevice(renderer->GetDevice());
}

//Called after the device is reset. Create D3DPOOL_DEFAULT resources here.
void Engine::OnResetDevice()
{
	if(game && renderer)
		game->OnResetDevice(renderer->GetDevice());
}

//Called after the device is lost. Release D3DPOOL_DEFAULT resources here.
void Engine::OnLostDevice()
{
	if(game)
		game->OnLostDevice();
}

//Called after the device is destroyed. Release D3DPOOL_MANAGED resources here.
void Engine::OnDestroyDevice()
{
	if(game)
		game->OnDestroyDevice();
}

//Updates the frame
void Engine::OnUpdateFrame()
{
	Time::Update();
	Input::Update();
	if(game && renderer)
	{
		game->HandleInput();
		game->OnUpdateFrame(renderer->GetDevice());
	}
}

//renders the current frame
void Engine::OnRenderFrame()
{
	if(!active || !renderer->GetDevice())
		return;

	//checking for lost device
	HRESULT hr = renderer->GetDevice()->TestCooperativeLevel();
	if(FAILED(hr))
	{
		if(hr == D3DERR_DEVICELOST)
		{
			Sleep(50);
			return;
		}
		else
		{
			OnLostDevice();
			if(renderer->Reset() == D3DERR_DEVICELOST)
			{
				Sleep(50);
				return;
			}
			else
				OnResetDevice();
		}
	}

	if(game)
		game->OnRenderFrame(renderer->GetDevice());
}

//Fullscreen toggle
void Engine::ToggleFullscreen()
{
	if(!active || !renderer)
		return;

	renderer->Windowed = !renderer->Windowed;
	if(renderer->Windowed) //set new style
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	else
	{
		ZeroMemory(&windowPlacement, sizeof(windowPlacement)); //saving prev position
		windowPlacement.length = sizeof(windowPlacement);
		GetWindowPlacement(hWnd, &windowPlacement);

		SetWindowLongPtr(hWnd, GWL_STYLE, WS_EX_TOPMOST); //fullscreen style
	}

	OnLostDevice();
	renderer->Reset();
	OnResetDevice();

	if(renderer->Windowed)
		SetWindowPlacement(hWnd, &windowPlacement); //using cached position
}

//toggling wireframe
void Engine::ToggleWireframe()
{
	fillMode = fillMode == D3DFILL_SOLID ? D3DFILL_WIREFRAME : D3DFILL_SOLID;
	renderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, fillMode);
}

//Event handler workaround - redirecting towards actual handler
LRESULT CALLBACK Engine::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_CREATE) //saving pointer to the class(this) which handles events
		SetWindowLongPtr(hWnd, GWL_USERDATA, (long)((CREATESTRUCT *)lParam)->lpCreateParams);

	Engine *engine = (Engine *)GetWindowLongPtr(hWnd, GWL_USERDATA);

	if(engine)
		return engine->WndProc(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Event handler
LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		if(renderer->GetDevice())
		{
			OnUpdateFrame();
			OnRenderFrame();
		}
		ValidateRect(hWnd, 0);
		return 0;
	case WM_SIZE:
		if(wParam == SIZE_MINIMIZED)
			active = false;
		else //resizing
		{
			active = true;
			windowWidth = LOWORD(lParam);
			windowHeight = HIWORD(lParam);
			if(renderer->GetDevice())
			{
				OnLostDevice();
				renderer->Reset();
				OnResetDevice();
				OnUpdateFrame();
				OnRenderFrame();
			}
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int Engine::GetWidth()
{
	return renderer->Windowed ? windowWidth : fullScreenWidth;
}

int Engine::GetHeight()
{
	return renderer->Windowed ? windowHeight : fullScreenHeight;
}