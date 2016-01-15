#pragma once
#include "Linker.h"
#include "Model.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
class Engine
{

private:
	int vertexAmount;
	int modelAmount;
	std::vector<Model*>* models;
	std::vector<Vertex>* verticesToRender;

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


	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
	void createShaders();

public:
	Engine();
	Engine(HWND* winHandle);
	

	virtual ~Engine();

	void addModel(Primitives type);

	void release();
	void run();
	void update();
	void render();
	void loadModels();
	void loadVertices();




};

