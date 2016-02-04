#include "Engine.h"




Engine::Engine()
{
}

Engine::Engine(HINSTANCE* hInstance,HWND* winHandle, Input* input)
{
	
	this->cam = new Camera();
	this->input = input;
	bool inputResult = input->initialize(hInstance, winHandle,this->cam);


	if (!inputResult)
	{	//If there is a problem creating the input, show a warning
		MessageBox(*winHandle, L"Cannot intialize input device", L"Error", MB_OK);
		
	}
	this->vertexAmount = 0;
	this->modelAmount = 0;
	this->lightAmount = 0;
	hr = CreateDirect3DContext(winHandle);

	createRasterizerState();

	setViewPort();

	createShaders();

	createConstantBuffers();

	//Load the models and get their vertices
	this->modelsColor = new std::vector<Model*>; //this will be an array 
	this->modelsTexture = new std::vector<Model*>;
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


	delete modelsTexture;
	delete modelsColor;
	delete lights;
	delete cam;
	
}

void Engine::release()
{
	
	//Release Color shaders
	gVertexLayoutColor->Release();
	gVertexShaderColor->Release();
	gPixelShaderColor->Release();
	gGeometryShaderColor->Release();

	//Release Texture shaders
	if(gVertexLayoutTexture != NULL)
		gVertexLayoutTexture->Release(); //If this crashes on shut down, is because there is no layout for texture yet
	
		
	gVertexShaderTexture->Release();
	gPixelShaderTexture->Release();
	gGeometryShaderTexture->Release();

	gBackbufferRTV->Release();
	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
	depthBuffer->Release();
	depthStencilView->Release();
	worldBuffer->Release();
	camBuffer->Release();
	lightBuffer->Release();

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
	ID3D11RasterizerState *pRasterizerState = nullptr;

	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE; //disable back face culling
	rastDesc.DepthClipEnable = true;

	hr = gDevice->CreateRasterizerState(&rastDesc, &pRasterizerState);

	if (SUCCEEDED(hr))
		gDeviceContext->RSSetState(pRasterizerState);

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
	D3D11_VIEWPORT vp;
	vp.Width = (float)WINDOW_WIDTH;
	vp.Height = (float)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->gDeviceContext->RSSetViewports(1, &vp);
	

}

void Engine::createShaders()
{

	createColorShaders();
	createTextureShaders();
	



}

void Engine::createTextureShaders()
{
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderTexture.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderTexture);

	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0} //We wont use Color here, that's why the offset is 32. were still using the same struct
		//Normals?

	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutTexture);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderTexture.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShaderTexture);
	pPs->Release();

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderTexture.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderTexture);
	pGS->Release();



}

void Engine::createColorShaders()
{

	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderColor.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderColor);

	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 }
		
	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutColor);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderColor.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShaderColor);
	pPs->Release();

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderColor.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderColor);
	pGS->Release();

}

void Engine::loadModels()
{

	this->addModel(PYRAMID);
	this->modelsColor->at(0)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(0)->setRotateState(true);
	this->modelsColor->at(0)->setTranslation(XMFLOAT3(0.5f, 0.0f, 0.0f));



	this->addModel(PYRAMID);
	this->modelsColor->at(1)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(1)->setRotateState(true);
	this->modelsColor->at(1)->setTranslation(XMFLOAT3(-0.5f, 0.0f, 0.0f));


	this->addModel(PYRAMID);
	this->modelsColor->at(2)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(2)->setRotateState(true);
	this->modelsColor->at(2)->setTranslation(XMFLOAT3(0.0f, 0.5f, 0.0f));


	this->addModel(PYRAMID);
	this->modelsColor->at(3)->setScale(XMFLOAT3(0.6f, 0.6f, 0.6f));
	this->modelsColor->at(3)->setRotation(XMFLOAT3(45.0f, 0.0f, 0.0f));
	this->modelsColor->at(3)->setTranslation(XMFLOAT3(-5.0f, -0.5f, 0.0f));

	this->addModel(PLANE);
	this->modelsColor->at(4)->setTranslation(XMFLOAT3(0.0f, -1.5f, 0.0f));
	this->modelsColor->at(4)->setRotation(XMFLOAT3(90.0f, 180.0f, 0.0f));
	this->modelsColor->at(4)->setScale(XMFLOAT3(50.0f, 50.0f, 50.0f));


	this->addModel(CUBE);
	this->modelsColor->at(5)->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	this->modelsColor->at(5)->setTranslation(XMFLOAT3(0.5f, 0.5f, 0.0f));

	this->addModel(OBJ);
	this->modelsTexture->at(0)->setTranslation(XMFLOAT3(5.0f, 0.0f, 0.0f));
	//this->modelsTexture->at(0)->setRotateState(true);

	this->addModel(OBJ, "BTHcube.obj");
	this->modelsTexture->at(1)->setTranslation(XMFLOAT3(0.0f, 0.0f, 5.0f));
	this->modelsTexture->at(1)->setRotateState(true);
}
void Engine::loadLights()
{
	this->addLight(POINTLIGHT);
}

void Engine::run()
{

	this->update();

	this->render();

	this->gSwapChain->Present(0, 0); //Change front and back buffer after rendering


}

void Engine::update()
{
	//updatera matrixBuffer här
	
	input->frame();
	camStruct.view = cam->getView();
	camStruct.projection = cam->getProjection();
	camStruct.camPos = cam->getCamPos();
	camStruct.camLook = cam->getCamLookAt();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	cameraConstantBuffer* temporary = (cameraConstantBuffer*)mappedResource.pData;

	*temporary = camStruct;

	this->gDeviceContext->Unmap(camBuffer, 0);

	this->gDeviceContext->GSSetConstantBuffers(1, 1, &camBuffer); 

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
	float clearColor[] = { 0, 0, 0, 1 };


	this->gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	////////////////////////////////////////////
	//Render The objects that use the COLOR shaders
	this->gDeviceContext->VSSetShader(gVertexShaderColor, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(gGeometryShaderColor, nullptr, 0);
	this->gDeviceContext->PSSetShader(gPixelShaderColor, nullptr, 0);
	this->gDeviceContext->IASetInputLayout(gVertexLayoutColor);
	
	for (int i = 0; i < this->modelsColor->size(); i++)
	{

	
		this->modelsColor->at(i)->update();
		this->modelsColor->at(i)->render();


	}
	////////////////////////////////////////////



	////////////////////////////////////////////
	//Render The objects that use the Texture shaders
	//
	this->gDeviceContext->VSSetShader(gVertexShaderTexture, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(gGeometryShaderTexture, nullptr, 0);
	this->gDeviceContext->PSSetShader(gPixelShaderTexture, nullptr, 0);
	this->gDeviceContext->IASetInputLayout(gVertexLayoutTexture);
	
	for (int i = 0; i < this->modelsTexture->size(); i++)
	{


		this->modelsTexture->at(i)->update();
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