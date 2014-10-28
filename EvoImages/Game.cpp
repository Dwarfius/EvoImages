#include "Game.h"
#include "Time.h"
#include "Input.h"

const int size = 200;

Game::Game()
{
	engine = NULL;
}

//sets the engine
void Game::SetEngine(Engine *engine)
{
	this->engine = engine;
}

//init application-specific resources here
bool Game::Init()
{
	Time::Start();
	srand((unsigned int)time(NULL));

	trees = NULL;

	camera.SetMaxVelocity(100);
	camera.SetPos(&D3DXVECTOR3(0, 10, -10));
	camera.LookAt(&D3DXVECTOR3(0, 0, 0));
	camera.Update();

	return true;
}

//clean up resources
void Game::Release()
{
	SAFE_RELEASE(engine);
}

//This callback function will be called immidiately after the D3DDevice is created. 
//This is the best location to create D3DPOOL_MANAGED resources. Resources created here
//should be released in the OnDestroyDevice callback.
void Game::OnCreateDevice(LPDIRECT3DDEVICE9 device)
{
	//texture creation
	for(int i=0; i<3; i++)
		D3DXCreateTexture(device, size, size, 0, 0, 
							D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &mainTexture[i]);

	//textRenderer creation
	D3DXCreateSprite(device, &sprite);
	text.Init(device, "Arial", 12);

	//creating images
	UpdateImages();
}

//This callback function will be called immidiately after the D3DDevice is created and Reset.
//This is the best location to create D3DPOOL_DEFAULT resources. Resources created here
//should be released in the OnLostDevice callback.
void Game::OnResetDevice(LPDIRECT3DDEVICE9 device)
{
	//world matrix
	camera.SetAspectRatio((float)engine->GetWidth() / (float)engine->GetHeight());
	device->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	//set up render states
	device->SetRenderState(D3DRS_FILLMODE, engine->GetFillMode());
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//text reset
	sprite->OnResetDevice();
	text.OnResetDevice();
}

//This callback function will be called immidiately after the device will enter the lost state
//and before the IDIRECT3DDEVICE9::Reset() is called. Resources created in OnResetDevice should
//be relased here (D3DPOOL_DEFAULT).
void Game::OnLostDevice()
{
	sprite->OnLostDevice();
	text.OnLostDevice();
}

//This callback funtion will be called immidiately after the device has been destroyed.
//Resources created in the OnCreateDevice should be released here (D3DPOOL_MANAGED).
void Game::OnDestroyDevice()
{
	text.Release();
	SAFE_RELEASE(sprite);
	for(int i=0; i<3; i++)
		SAFE_RELEASE(mainTexture[i]);
}

//Updates the current frame
void Game::OnUpdateFrame(LPDIRECT3DDEVICE9 device)
{
	//updating camera movement
	camera.Update();
}

//Renders the current frame
void Game::OnRenderFrame(LPDIRECT3DDEVICE9 device)
{
	//rendering using camera view
	device->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	device->BeginScene();

	//render everything here
	DWORD color = D3DCOLOR_XRGB(255, 255, 255);
	
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	for(int i=0; i<3; i++)
	{
		int index = i*3;
		int yPos = (size + 50) * i;
		sprite->Draw(mainTexture[i], NULL, NULL, &D3DXVECTOR3(0, (float)yPos, 0), color);
		
		text.Print(&(treeStrings[index])[0], size + 20, yPos + size/2 - 15, color);
		text.Print(&(treeStrings[index+1])[0], size + 20, yPos + size/2, color);
		text.Print(&(treeStrings[index+2])[0], size + 20, yPos + size/2 + 15, color);
	}
	sprite->End();

	device->EndScene();
	device->Present(0, 0, 0, 0);
}

//Key event handler, called before Game::Update()
void Game::HandleInput()
{
	if(Input::GetKeyDown(DIK_ESCAPE))
		PostQuitMessage(0);
	if(Input::GetKeyDown(DIK_F1) && engine)
		engine->ToggleFullscreen();
	if(Input::GetKeyDown(DIK_F2) && engine)
		engine->ToggleWireframe();
	if(Input::GetKeyDown(DIK_F3))
		UpdateImages();
}

void Game::UpdateImages()
{
	if(trees)
	{
		delete [] trees;
		delete [] treeStrings;
	}

	trees = new Tree[9];
	treeStrings = new string[9];
	
	for(int i=0; i<3; i++)
	{
		int index = i*3;
		treeStrings[index] = "Red = " + trees[index].GetString();
		treeStrings[index+1] = "Green = " + trees[index+1].GetString(); 
		treeStrings[index+2] = "Blue = " + trees[index+2].GetString();

		D3DLOCKED_RECT lockedRect = {0};
		if(FAILED(mainTexture[i]->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD)))
		{
			MessageBox(0, "Error locking texture rect", "Error", 0);
			return;
		}
		DWORD *bytes = (DWORD *)lockedRect.pBits;
		for(int y=0; y<size; y++)
		{
			for(int x=0; x<size; x++)
			{
				DWORD j = y * lockedRect.Pitch / 4 + x;
				bytes[j] = D3DCOLOR_XRGB((int)trees[index].GetResult(x, y) % 256,
										 (int)trees[index+1].GetResult(x, y) % 256,
										 (int)trees[index+2].GetResult(x, y) % 256);
			}
		}
		mainTexture[i]->UnlockRect(0);
	}
}

//entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game *game = new Game();
	Engine *engine = new Engine((BaseGame *)game);

	game->SetEngine(engine);

	if(!game->Init())
		return 0;

	//initing engine and window
	if(!engine->Init("Engine", hInstance, 640, 3*(size + 50), true))
		return 0;

	//launching the game loop
	engine->Run();

	SAFE_RELEASE(game);

	return 0;
}