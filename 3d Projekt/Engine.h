#pragma once
#include "Linker.h"
#include "Model.h"
class Engine
{

private:

	HRESULT hr;
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11Buffer* gVertexBuffer = nullptr;

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	ID3D11DepthStencilView* depthStencilView = nullptr;

	ID3D11InputLayout* gVertexLayout = nullptr;

	//Shaders
	ID3D11VertexShader* gVertexShader = nullptr;
	//ID3D11GeometryShader* gGeometryShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;

	Model* models;

	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
	void createShaders();
public:
	Engine();
	Engine(HWND* winHandle);
	virtual ~Engine();

	void release();
	void run();
	void update();
	void render();

	void newFunction();




};

