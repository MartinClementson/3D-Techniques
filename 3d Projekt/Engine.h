#pragma once
#include "Linker.h"
#include "Model.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
#include "SkyBox.h"
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
	


	//Shaders for color
	ID3D11VertexShader* gVertexShaderColor = nullptr;
	ID3D11GeometryShader* gGeometryShaderColor = nullptr;
	ID3D11PixelShader* gPixelShaderColor = nullptr;
	ID3D11InputLayout* gVertexLayoutColor = nullptr;

	//Shaders for Texture
	ID3D11VertexShader* gVertexShaderTexture = nullptr;
	ID3D11GeometryShader* gGeometryShaderTexture = nullptr;
	ID3D11PixelShader* gPixelShaderTexture = nullptr;
	ID3D11InputLayout* gVertexLayoutTexture = nullptr;

	//SamplerState
	ID3D11SamplerState* gSampleState = nullptr;

	
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
	void createShaders();
	void createTextureShaders();
	void createColorShaders();
	void createRasterizerState();
	void createConstantBuffers();

	void errorMsg(std::string msg); //a function to show errors
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
	void addModel(Primitives type, std::string filename); //overload, for OBJ

	void updateLight();
	void loadLights();
	void addLight(lightTypes type);
	




};

