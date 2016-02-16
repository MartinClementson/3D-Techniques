#include "Engine.h"




Engine::Engine()
{
}

Engine::Engine(HINSTANCE* hInstance,HWND* winHandle, Input* input)
{
		
	this->cam = new Camera();
	this->sky = new SkyBox();

	this->renderTexture = new RenderTexture();
	this->shaderManager = new ShaderManager();
	this->dynCubeMap = new DynamicCubeMap();
	this->input = input;
	this->wndHandle = winHandle;

	bool inputResult = input->initialize(hInstance, winHandle,this->cam);
	if (!inputResult)
	{	//If there is a problem creating the input, show a warning
		
		errorMsg("Cannot intialize input device");
	}
	this->vertexAmount = 0;
	this->modelAmount = 0;
	this->lightAmount = 0;

	hr = CreateDirect3DContext(winHandle);
	if (FAILED(hr))
		errorMsg("Failed to create Direct3D Context");



	createRasterizerState();

	setViewPort();

	//createShaders();

	//create all the shaders
	if (!shaderManager->Init(this->gDevice,this->gDeviceContext))
	{
		errorMsg("Failed to initialize the shader manager");
		delete shaderManager;

	}

	createConstantBuffers();

	//create the skybox
	if (!sky->Init(this->gDeviceContext, this->gDevice, this->worldBuffer, &this->worldStruct))
	{
		errorMsg("Failed to initialize Skybox");
		delete sky;
	}


	if (!renderTexture->Init(this->gDevice, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		errorMsg("Failed to initalize Render Texture");
		delete this->renderTexture;
	}

	if (!dynCubeMap->Init(gDevice, gDeviceContext))
	{
		errorMsg("Failed to initialize dynamic cube mapping");

		delete this->dynCubeMap;
	}


	//Load the models and get their vertices
	this->modelsColor = new std::vector<Model*>; 
	this->modelsTexture = new std::vector<Model*>;
	this->cubeMapModels = new std::vector<Model*>;
	this->lights = new std::vector<Light>;
	
	loadModels();
	loadLights();
	

	
	
	
	
}



Engine::~Engine()
{
	for (int i = 0; i < modelsColor->size(); i++)
	{
		delete modelsColor->at(i); 

	}
	for (int i = 0; i < modelsTexture->size(); i++)
	{
		delete modelsTexture->at(i);

	}

	for (int i = 0; i < cubeMapModels->size(); i++)
	{
		delete cubeMapModels->at(i);

	}
	delete shaderManager;
	delete renderTexture;
	delete dynCubeMap;
	delete sky;
	delete modelsTexture;
	delete modelsColor;
	delete cubeMapModels;
	delete lights;
	delete cam;
	
}

void Engine::release()
{
	
	////Release Color shaders
	//gVertexLayoutColor->Release();
	//gVertexShaderColor->Release();
	//gPixelShaderColor->Release();
	//gGeometryShaderColor->Release();

	//Release Texture shaders
	//if(gVertexLayoutTexture != NULL)
	//	gVertexLayoutTexture->Release(); //If this crashes on shut down, is because there is no layout for texture yet
	//
	if (gSampleState != nullptr)
		gSampleState->Release();
		
	/*gVertexShaderTexture->Release();
	gPixelShaderTexture->Release();
	gGeometryShaderTexture->Release();
	*/

	input->Shutdown();
	dynCubeMap->Release();
	shaderManager->Release();
	renderTexture->Release();
	gRasterizerState->Release();
	gBackbufferRTV->Release();
	gSwapChain->Release();
	

	
	gDeviceContext->ClearState();
	gDeviceContext->Release();
	gDevice->Release();
	
	depthBuffer->Release();
	depthState->Release();
	depthStencilView->Release();


	worldBuffer->Release();
	camBuffer->Release();
	lightBuffer->Release();
	sky->Release();

	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

}

void Engine::createConstantBuffers()
{
	//Creating the Camera constant buffer
	CD3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(cameraConstantBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDesc, nullptr, &camBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->GSSetConstantBuffers(1, 1, &camBuffer); //change into geometry shader later <------------------------


	//Creating world constant buffer																 
	CD3D11_BUFFER_DESC bufferDescWorld;
	ZeroMemory(&bufferDescWorld, sizeof(bufferDescWorld));
	bufferDescWorld.ByteWidth = sizeof(worldConstantBuffer);
	bufferDescWorld.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescWorld.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescWorld.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescWorld.MiscFlags = 0;
	bufferDescWorld.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescWorld, nullptr, &worldBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer); //change into geometry shader later

	CD3D11_BUFFER_DESC bufferDescLight;
	ZeroMemory(&bufferDescLight, sizeof(bufferDescLight));
	bufferDescLight.ByteWidth = sizeof(lightConstantBuffer);
	bufferDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescLight.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescLight.MiscFlags = 0;
	bufferDescLight.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescLight, nullptr, &lightBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);


}

void Engine::createRasterizerState()
{

	//Setting up the Rasterizer state
	//needed in this project to disable the built in back face culling
	D3D11_RASTERIZER_DESC rastDesc;
	

	ZeroMemory(&rastDesc, sizeof(rastDesc));

	if(WIREFRAME)
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	else
		rastDesc.FillMode = D3D11_FILL_SOLID;


	rastDesc.CullMode = D3D11_CULL_BACK;//D3D11_CULL_NONE; //disable back face culling
	rastDesc.DepthClipEnable = true;

	hr = gDevice->CreateRasterizerState(&rastDesc, &gRasterizerState);

	if (SUCCEEDED(hr))
		gDeviceContext->RSSetState(gRasterizerState);
	else
		errorMsg("Failed to create Rasterizer state");

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
	scd.BufferDesc.RefreshRate.Numerator = 60; //fps cap
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->gSwapChain,
		&this->gDevice,
		NULL,
		&this->gDeviceContext);
	
	hr = gDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
	if (FAILED(hr)) errorMsg("ERROR INITIALIZING DEBUG");
	//Here goes depth buffer
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = WINDOW_WIDTH;
	desc.Height = WINDOW_HEIGHT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 4;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = gDevice->CreateTexture2D(&desc, 0, &depthBuffer);

	hr = gDevice->CreateDepthStencilView(depthBuffer, 0, &depthStencilView);

	//Create depth state
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //Default
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS; //Default

	gDevice->CreateDepthStencilState(&dssDesc, &depthState);




	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackbufferRTV);
		pBackBuffer->Release();

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, depthStencilView); 


	}

	return hr;

}

void Engine::setViewPort()
{
	
	vp.Width = (float)WINDOW_WIDTH;
	vp.Height = (float)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->gDeviceContext->RSSetViewports(1, &vp);
	

}


void Engine::loadModels()
{

#pragma region triangleHierarcy //#pragma region just makes the text collapsable
	this->addModel(PYRAMID);
	//this->modelsColor->at(0)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(0)->setRotateState(true);
	this->modelsColor->at(0)->setTranslation(XMFLOAT3(-10.0f, 1.0f, 4.0f));


	this->addModel(PYRAMID);
	//this->modelsColor->at(1)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(1)->setRotateState(true);
	this->modelsColor->at(1)->setRotationSpeed(1.0);
	this->modelsColor->at(1)->setTranslation(XMFLOAT3(-12.0f, 0.0f, 4.0f));
	this->modelsColor->at(1)->setPivotPoint(this->modelsColor->at(0)->getTranslation());


	this->addModel(PYRAMID);
	//this->modelsColor->at(2)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(2)->setRotateState(true);
	this->modelsColor->at(2)->setRotationSpeed(1.0);
	this->modelsColor->at(2)->setTranslation(XMFLOAT3(-8.0f, 0.0f, 4.0f));
	this->modelsColor->at(2)->setPivotPoint(this->modelsColor->at(0)->getTranslation());

#pragma endregion end of hierarcy

	this->addModel(PYRAMID);
	this->modelsColor->at(3)->setScale(XMFLOAT3(0.6f, 0.6f, 0.6f));
	this->modelsColor->at(3)->setRotation(XMFLOAT3(45.0f, 0.0f, 0.0f));
	this->modelsColor->at(3)->setRotationSpeed(-0.5);
	this->modelsColor->at(3)->setTranslation(XMFLOAT3(-5.0f, -1.0f, 0.0f));
	this->modelsColor->at(3)->setPivotPoint(XMFLOAT3(0.0f, 0.0f, 1.0f));
	this->modelsColor->at(3)->setRotateState(true);


	this->addModel(CUBE);
	this->modelsColor->at(4)->setScale(XMFLOAT3(10.f, 10.0f, 10.0f));
	this->modelsColor->at(4)->setTranslation(XMFLOAT3(0.5f, 1.0f, 30.0f));
	this->modelsColor->at(4)->setRotateState(true);


	//this->addModel(OBJ);
	//this->modelsTexture->at(0)->setTranslation(XMFLOAT3(5.0f, 0.0f, 0.0f));
	////this->modelsTexture->at(0)->setRotateState(true);


	this->addModel(OBJ, "BTHcube.obj");
	this->modelsTexture->at(1)->setTranslation(XMFLOAT3(0.0f, 1.0f, 5.0f));
	this->modelsTexture->at(1)->setRotateState(true);
	this->modelsTexture->at(1)->setRotationSpeed(1.0f);
	this->modelsTexture->at(1)->setPivotPoint(XMFLOAT3(0.0f, 0.0f, 1.0f));

	this->addModel(OBJ, "plane.obj");
	this->modelsTexture->at(2)->setTranslation(XMFLOAT3(0.0f, -2.0f, 0.0f));

	this->addModel(OBJ, "Sphere.obj",CUBEMAPSHADER);
	this->cubeMapModels->at(0)->setTranslation(XMFLOAT3(0.0f, 1.0f, 1.0f));



}
void Engine::loadLights()
{
	this->addLight(POINTLIGHT);
}

void Engine::run()
{

	this->update();

	this->render();

	this->gSwapChain->Present(VSYNC, 0); //Change front and back buffer after rendering


}

void Engine::update()
{
	
	
	input->frame(); //Check for user input



	this->updateCamera(this->cam); //This needs to be moved
	//update all the models
	for (int i = 0; i < this->modelsColor->size(); i++)
	{

		this->modelsColor->at(i)->update();
	}
	for (int i = 0; i < this->modelsTexture->size(); i++)
	{

		this->modelsTexture->at(i)->update();
	}
	for (int i = 0; i < this->cubeMapModels->size(); i++)
	{

		this->cubeMapModels->at(i)->update();
	}

	
	
	this->updateLight();

}
void Engine::updateLight()
{
	//updating the lightConstantBuffer
	for (int i = 0; i < this->lightAmount; i++) //fix later so that the lights will not overwrite eachother if there are more lights than one
	{
		lightStruct.lightPosition = lights->at(i).getLightPosition();
		lightStruct.lightColor = lights->at(i).getLightColor();
		lightStruct.intensity = lights->at(i).getIntensity();

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		this->gDeviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		lightConstantBuffer* temporaryLight = (lightConstantBuffer*)mappedResource.pData;
		*temporaryLight = lightStruct;

		this->gDeviceContext->Unmap(lightBuffer, 0);

		this->gDeviceContext->PSSetConstantBuffers(1, 1, &lightBuffer);
	}
}

void Engine::render()
{


	//In this function different render passes will be made.
	//The scene is rendered in the renderScene() function
	//the loop specifies  wich pass is being rendered
	//It doesent really need to be a loop. Possible subject of change


	//The first pass (added after the comment above) is the dynamic cubeMap
	//This is independant from the later passes, as this renders from a whole set of different cameras and not the player camera

	for (int j = 0; j < cubeMapModels->size(); j++)					//looping is a little overkill, since we only have one object that uses this.
																	//also, there is no support for multiple objects yet.
																	//for multiple objects, we need to implement multiple textures in the class
	{
		XMFLOAT3 position = cubeMapModels->at(j)->getTranslation(); //Get the position of the reflective object
		this->dynCubeMap->Render(position, this);					//this function will set the viewport,depthbuffer,backbuffer, back to normal when it's done
	}

	this->updateCamera(this->cam);									//this sets the camera to the const buffer, Replacing the cameras
																	// used with dynamic cube mapping

	float clearColor[] = { 0, 0, 0, 1 };

	this->gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);


	for (int i = 0; i < 2; i++)
	{

		if (i == 0) //Render to texture. first loop
		{
			renderTexture->SetRenderTarget(&*gDeviceContext, depthStencilView);
			renderTexture->ClearRenderTarget(gDeviceContext, depthStencilView, 0, 1, 0, 0);
			//this->shaderManager->setActiveShaders(SKYBOXSHADER);
			
			
			renderScene();
			
		}

		else //render to backbuffer/screen. next loop
		{

			this->gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, depthStencilView);	//Set backbuffer as render target
			
			/////////////////////////////////
			//Set the renderToTexture as a subresource
			ID3D11ShaderResourceView* shaderResourceViewz = renderTexture->GetShaderResourceView();
			//Applu the renderTexture to the pixel shader
			this->gDeviceContext->PSSetShaderResources(2, 1, &shaderResourceViewz);
			//////////////////////////////////

			renderScene();											//Render scene
			this->shaderManager->setActiveShaders(CUBEMAPSHADER);	//Apply dynamic cube map shader
			this->cubeMapModels->at(0)->render();					//Render the model using the dynamix cube map

			//The render texture needs to be taken of as a shader resource
			//This is so that we can write to it again in the next frame
			//When we were not doing this, DirectX debug mode sent alot of warnings
			ID3D11ShaderResourceView * tab[1];
			tab[0] = NULL;
			this->gDeviceContext->PSSetShaderResources(2, 1, tab);

		}



		

	}

}


void Engine::renderScene() // This function will render the scene, no matter the render pass used
{
	//Render skybox
	this->shaderManager->setActiveShaders(SKYBOXSHADER);
	sky->update(this->cam->getCamPos()); //Send in the position of the camera. The skybox needs to be centered around the camera
	sky->render();




	gDeviceContext->OMSetDepthStencilState(depthState, 0);
	gDeviceContext->RSSetState(gRasterizerState);

	////////////////////////////////////////////
	//Render The objects that use the COLOR shaders
	this->shaderManager->setActiveShaders(COLORSHADER);

	for (int i = 0; i < this->modelsColor->size(); i++)
	{

		
		this->modelsColor->at(i)->render();

	}
	////////////////////////////////////////////



	////////////////////////////////////////////
	//Render The objects that use the Texture shaders
	//

	this->shaderManager->setActiveShaders(TEXTURESHADER);

	for (int i = 0; i < this->modelsTexture->size(); i++)
	{
		 
		this->modelsTexture->at(i)->render();
	}
	//
	////////////////////////////////////////////

}

void Engine::addModel(Primitives type)
{
	//"Primitives" datatype is an Enumeration of our 3d primitives.
	//With the "type" parameter we choose what kind of model will be added

	//In the switch we create the model accordingly
	switch (type)
	{
		case CUBE:
		{
			this->modelsColor->push_back(new Cube(this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
			this->modelAmount += 1;
			break;
		}

		case PLANE:
		{
			this->modelsColor->push_back(new Plane(this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
			this->modelAmount += 1;
			break;
		}

		case PYRAMID:
		{
			this->modelsColor->push_back(new Pyramid(this->gDevice,this->gDeviceContext,this->worldBuffer,&this->worldStruct));
			this->modelAmount += 1;
			break;
		}

		case OBJ :
		{
			this->modelsTexture->push_back(new Model(std::string("test.obj"),this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
			this->modelAmount += 1;
			break;

		}

	}


}

void Engine::addModel(Primitives type, std::string filename) // This is an overloaded function, For OBJ
{
	if (type == OBJ)
	{
		this->modelsTexture->push_back(new Model(filename, this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
		this->modelAmount += 1;

	}
	else {

		this->addModel(type);
	}


}


void Engine::addModel(Primitives type, std::string filename,ShaderTypes shaderToBeUsed) // This is an overloaded function, For OBJ
{

	/* This function is another overload to "addModel" function, Here you can specify what shader the object are to be used with
	this works on those that have the same input layout as the Textureshader,
	for now. it is, Textureshaders, cubemap shaders. (Skybox could also work, but it is not what we want)*/
	if (type == OBJ)
	{
		switch (shaderToBeUsed)
		{
		case TEXTURESHADER:
			this->modelsTexture->push_back(new Model(filename, this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
			this->modelAmount += 1;
			break;
		
		case CUBEMAPSHADER:
			this->cubeMapModels->push_back(new Model(filename, this->gDevice, this->gDeviceContext, this->worldBuffer, &this->worldStruct));
			this->modelAmount += 1;
			break;



		}


	}
	else {

		this->addModel(type);
	}


}

void Engine::updateCamera(Camera* cameraToRender)
{
	camStruct.view = cameraToRender->getView();
	camStruct.projection = cameraToRender->getProjection();
	camStruct.camPos = cameraToRender->getCamPos();
	camStruct.camLook = cameraToRender->getCamLookAt();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	cameraConstantBuffer* temporary = (cameraConstantBuffer*)mappedResource.pData;

	*temporary = camStruct;

	this->gDeviceContext->Unmap(camBuffer, 0);

	//&this->gDeviceContext->GSSetConstantBuffers(1, 1, &camBuffer); <--- im not sure why it works with this commented


}


void Engine::addLight(lightTypes type)
{
	//following the same format as model
	switch (type)
	{
		case POINTLIGHT:
		{
			this->lights->push_back(Light());  //Fix later so that you can reach the custom constructor
			this->lightAmount += 1;
			break;
		}
	}
}


void Engine::errorMsg(std::string msg)
{
	

	//Convert string to LPCWSTR type
	std::wstring stemp = std::wstring(msg.begin(), msg.end());
	LPCWSTR sw = stemp.c_str();

	MessageBox(*wndHandle, sw, L"Error", MB_ICONERROR | MB_OK);
	throw; //This generates an exception. because there isn't really any exception to throw
	//this allows for the debugger to break, and the programmer can look at the call stack to find the issue
}