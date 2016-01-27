#pragma once
#include "Linker.h"
#include "Model.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
class Engine
{

private:
	Camera* cam; 
	int vertexAmount;
	int modelAmount;
	int lightAmount;
	std::vector<Model*>* models;
	std::vector<Light>* lights;
	
	Input* input;


	HRESULT hr;
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11Buffer* gVertexBuffer = nullptr;

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D *depthBuffer = nullptr;
	

	ID3D11InputLayout* gVertexLayout = nullptr;

	//Shaders
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11GeometryShader* gGeometryShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;


	//Constant buffers
	worldConstantBuffer worldStruct;
	ID3D11Buffer* worldBuffer = nullptr;

	cameraConstantBuffer camStruct;
	ID3D11Buffer* camBuffer = nullptr;

	lightConstantBuffer lightStruct;
	ID3D11Buffer* lightBuffer = nullptr;

	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
	void createShaders();

	void createConstantBuffers();
public:
	Engine();
	Engine(HINSTANCE* hInstance,HWND* winHandle, Input* input);
	

	virtual ~Engine();


	void release();
	void run();
	void update();
	void render();

	void loadModels();
	void addModel(Primitives type);

	void updateLight();
	void loadLights();
	void addLight(lightTypes type);
	




};

