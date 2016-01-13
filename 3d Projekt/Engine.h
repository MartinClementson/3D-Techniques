#pragma once
#include "Linker.h"
class Engine
{

private:

	HRESULT hr;
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	ID3D11DepthStencilView* depthStencilView = nullptr;

	ID3D11InputLayout* gVertexLayout = nullptr;

	Model* models;

	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
public:
	Engine();
	Engine(HWND* winHandle);
	virtual ~Engine();

	void update();
	void render();





};

