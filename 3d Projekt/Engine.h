#pragma once
#pragma region Includes
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
#include "Terrain.h"
#include "QuadTree.h"
#include "Overlay.h"
#include "md5Model.h"
#include "ComputeShaderClass.h"
class DynamicCubeMap; //forward declaration

#pragma endregion


class Engine
{

private:
#pragma region Custom Classes
	Terrain* heightMap = nullptr;
	Camera* cam = nullptr; 
	Camera* miniMapCam = nullptr;
	Input* input = nullptr;
	SkyBox* sky = nullptr;
	ShaderManager* shaderManager = nullptr;
	DynamicCubeMap* dynCubeMap = nullptr;
	RenderTexture* miniMapTexture;
	QuadTree* quadTreeTerrain;
	md5Model* animationModel;
	ComputeShaderClass* postProcess;
	
	RenderTexture* postProcessTexture = nullptr; //We use this if we want post processing. 
												 //We render the whole scene to this texture. 
												 //Then we send it into the compute shader for post processing
	
	Overlay* ui;
	bool miniMap = true;
	bool walkTerrain = false;
	bool postProcessActive = false;

#pragma endregion

	int vertexAmount;
	int modelAmount;
	int lightAmount;
	UINT drawCount;

#pragma region Model arrays
	std::vector<Model*>* modelsColor;
	std::vector<Model*>* modelsTexture;
	std::vector<Model*>* cubeMapModels; //Models to use the Dynamic cube map as reflection. This is very demanding, and should not be many objects
	std::vector<Light>* lights;
#pragma endregion
	
#pragma region COM objects/DirectX
	ID3D11Debug* debug; //Debug COM
	HRESULT hr; //Result variable


	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;
	ID3D11UnorderedAccessView* gBackBufferUAV = nullptr; //This is needed for the compute shader, since it doesent use "renderTargetView", but it's connected to the same texture
	ID3D11ShaderResourceView *BackBufferTexture = nullptr;

	ID3D11RenderTargetView** currentRTV; //This is a way for us to keep trackof the active render target view.
										 //It's needed since we have the post process render target now. 
										 // And the dynamic cube map needs to know which render target to reset to
										 // if post process is on = reset to the RenderTexture object "postProcessTexture"
										 // else if it's off : reset to the back buffer rtv
										 //the dynamic cube map just calls the engine function "getRenderTargetView()" 
										 //this double pointer returns the current active RTV to be used.
	//Depth stencil
	ID3D11DepthStencilState* depthState = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D *depthBuffer = nullptr;
	
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

	//pixelShaderConstants pixelStateStruct;
	//ID3D11Buffer* pixelStateBuffer =nullptr; //This buffer is for the booleans in the pixel shader, normal= on/off, etc
	//ID3D11Buffer* objectMaterialBuffer; //THis buffer is the material of the object being rendered/ kd,ka,tf,ni
	//
	modelBuffers* modelBuffer;

	D3D11_VIEWPORT vp; //Viewport
	D3D11_VIEWPORT miniMapVP;

	HWND* wndHandle;
#pragma endregion

#pragma region Private functions
	HRESULT CreateDirect3DContext(HWND* wndHandle);
	void setViewPort();
	void sendPixelStateToBuffer();
	void createRasterizerState();
	void createConstantBuffers();
	void errorMsg(std::string msg); //a function to show errors
#pragma endregion

public:
	Engine();
	Engine(HINSTANCE* hInstance,HWND* winHandle, Input* input);
	

	void renderScene(Camera *Camera);
	virtual ~Engine();


	void release();
	void run(float deltaTime);
	void update(float deltaTime);
	void render();
#pragma region Adding models
	void loadModels();
	void addModel(Primitives type);
	void addModel(Primitives type, std::string filename); //overload, for OBJ
	void addModel(Primitives type, std::string filename, ShaderTypes shaderToBeUsed); //overload, for OBJ
#pragma endregion
	void updatePixelShaderState();
	void updateCamera(Camera* cameraToRender);
	void updateLight();
	void loadLights();
	void addLight(lightTypes type);
	

	int getQTdrawCount() { return this->drawCount; }//this->quadTreeTerrain->GetDrawCount(); }
	D3D11_VIEWPORT getViewPort() { return this->vp; };
	ID3D11RenderTargetView* getRenderTargetView() { return *this->currentRTV; };
	ID3D11DepthStencilView* getDepthStencilView() { return this->depthStencilView; };


	void setDistanceFog(BOOL x) { this->modelBuffer->renderstate->distanceFog = x; sendPixelStateToBuffer(); };
	void setNormalMap(BOOL x) { this->modelBuffer->renderstate->normalMap = x; sendPixelStateToBuffer(); };
	
};

