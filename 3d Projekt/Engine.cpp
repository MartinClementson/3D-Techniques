#include "Engine.h"



Engine::Engine()
{
}

Engine::Engine(HWND* winHandle)
{
	hr = CreateDirect3DContext(winHandle);
	setViewPort();
	


}



Engine::~Engine()
{
}


HRESULT Engine::CreateDirect3DContext(HWND* wndHandle) 
{
	//Swap chain description
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = *wndHandle;
	scd.SampleDesc.Count = 4;
	scd.Windowed = WINDOWED;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->gSwapChain,
		&this->gDevice,
		NULL,
		&this->gDeviceContext);
	

	//Here goes depth buffer


	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackbufferRTV);
		pBackBuffer->Release();

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, NULL); //NULL switches to depthstencilView later


	}

	return hr;

}

void Engine::setViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)WINDOW_WIDTH;
	vp.Height = (float)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->gDeviceContext->RSSetViewports(1, &vp);


}