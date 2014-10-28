#include "Renderer.h"

Renderer::Renderer()
{
	d3d = NULL;
	d3ddev = NULL;
}

bool Renderer::Init(HWND hWnd, bool windowed)
{
	this->hWnd = hWnd;
	this->Windowed = windowed;

	Release();

	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);

	//checking up the vertex processing capacity
	DWORD vertexProcessing = 0;
	if(d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if(d3dcaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
			vertexProcessing |= D3DCREATE_PUREDEVICE;
	}
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if(!CreatePresParams())
	{
		Release();
		return false;
	}

	HRESULT hRes = d3d->CreateDevice(D3DADAPTER_DEFAULT, //default graphics adaptor
					  D3DDEVTYPE_HAL, //use hardware renderer
					  hWnd, //current window handle
					  vertexProcessing,
					  &d3dpp,
					  &d3ddev);
	if(FAILED(hRes))
	{
		Release();
		MessageBox(NULL, "Render Init Failed", "Error", 0);
		return false;
	}
	
	return true;
}

bool Renderer::CreatePresParams()
{
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	D3DFORMAT adapterFormat = Windowed ? d3ddm.Format : D3DFMT_X8R8G8B8;
	if(SUCCEEDED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
										D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	else if(SUCCEEDED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
										D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	else if(SUCCEEDED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat,
										D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	else
	{
		MessageBox(NULL, "Depth Stencil Format init failure", "Error", 0);
		return false;
	}

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = Windowed;
	d3dpp.BackBufferWidth = Windowed ? 0 : d3ddm.Width; //0 means will be changed later
	d3dpp.BackBufferHeight = Windowed ? 0 : d3ddm.Height;
	d3dpp.BackBufferFormat = adapterFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.FullScreen_RefreshRateInHz = Windowed ? 0 : d3ddm.RefreshRate;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //back buffer swapping method

	return true;
}

HRESULT Renderer::Reset()
{
	CreatePresParams();
	return d3ddev->Reset(&d3dpp);
}

void Renderer::Release()
{
	SAFE_RELEASE(d3ddev);
	SAFE_RELEASE(d3d);
}