#pragma once
#include "Linker.h"
#include "Model.h"
#include "RenderTexture.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
#include "SkyBox.h"
#include "ShaderManager.h"
#include "DynamicCubeMap.h"
class DynamicCubeMap;
class Engine
{

private:
	Camera* cam; 
	SkyBox* sky;

	int vertexAmount;
	int modelAmount;
	int lightAmount;
	std::vector<Model*>* modelsColor;
	std::vector<Model*>* modelsTexture;
	std::vector<Model*>* cubeMapModels; //Models to use the Dynamic cube map as reflection. This is very demanding, and should not be many objects
	std::vector<Light>* lights;
	
	Input* input;
	ID3D11Debug* debug;

	HRESULT hr;
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	//Depth stencil
	ID3D11DepthStencilState* depthState = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D *depthBuffer = nullptr;
	
	ShaderManager* shaderManager = nullptr;
	DynamicCubeMap* dynCubeMap = nullptr;

	RenderTexture* renderTexture;
	//SamplerState
	ID3D11SamplerState* gSampleState = nullptr;

	//RasterizerState
	ID3D11RasterizerState *gRasterizerState = nullptr;
	
	//Constant buffers
	worldConstantBuffer worldStruct;
	ID3D11Buffer* worldBuffer = nullptr;

	cameraConstantBuffer camStruct;
	ID3D11Buffer* camBuffer = nullptr;

	lightConstantBuffer lightStruct;
	ID3D11Buffer* lightBuffer = nullptr;

	HWND* wndHandle;

	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
	
	void createRasterizerState();
	void createConstantBuffers();
	D3D11_VIEWPORT vp; //Viewport
	void errorMsg(std::string msg); //a function to show errors

public:
	Engine();
	Engine(HINSTANCE* hInstance,HWND* winHandle, Input* input);
	

	void renderScene();
	virtual ~Engine();


	void release();
	void run();
	void update();
	void render();

	void loadModels();
	void addModel(Primitives type);
	void addModel(Primitives type, std::string filename); //overload, for OBJ
	void addModel(Primitives type, std::string filename, ShaderTypes shaderToBeUsed); //overload, for OBJ
	void updateCamera(Camera* cameraToRender);
	void updateLight();
	void loadLights();
	void addLight(lightTypes type);
	


	D3D11_VIEWPORT getViewPort() { return this->vp; };
	ID3D11RenderTargetView* getRenderTargetView() { return this->gBackbufferRTV; };
	ID3D11DepthStencilView* getDepthStencilView() { return this->depthStencilView; };

};

