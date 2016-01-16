#include "Engine.h"




Engine::Engine()
{
}

Engine::Engine(HWND* winHandle)
{
	this->vertexAmount = 0;
	this->modelAmount = 0;
	hr = CreateDirect3DContext(winHandle);

	setViewPort();

	createShaders();

	createConstantBuffers();

	//Load the models and get their vertices
	this->models = new std::vector<Model*>; //this will be an array 
	
	loadModels();
	

	
	
	
	
}



Engine::~Engine()
{
	for (int i = 0; i < models->size(); i++)
	{
		delete models->at(i); 

	}
	delete models;
	
}

void Engine::release()
{
	
	
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gGeometryShader->Release();
	gBackbufferRTV->Release();
	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
	depthBuffer->Release();
	depthStencilView->Release();
}

void Engine::createConstantBuffers()
{
	//Creating the matrix constant buffer
	CD3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(worldViewProjection);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDesc, nullptr, &matrixBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer); //change into geometry shader later


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

	//create vertex shader

	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0}
		//Normals and UV goes here

	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShader);
	pPs->Release();

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShader.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);
	pGS->Release();


}

void Engine::loadModels()
{
	
	
	this->addModel(PYRAMID);
	this->addModel(PLANE);
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
	float static angle = 0; //<----- just temporary to test rotation
	angle += 0.0001;
	DirectX::XMStoreFloat4x4(&matrixStruct.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(angle)));
	//DirectX::XMStoreFloat4x4(&matrixStruct.world, DirectX::XMMatrixIdentity());
	matrixStruct.view = cam.getView();
	matrixStruct.projection = cam.getProjection();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	worldViewProjection* temporary = (worldViewProjection*)mappedResource.pData;

	*temporary = matrixStruct;

	this->gDeviceContext->Unmap(matrixBuffer, 0);

	this->gDeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer); //change to geometry shader later
	
}

void Engine::render()
{
	float clearColor[] = { 0, 0, 0, 1 };


	this->gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	this->gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	//this->gDeviceContext->PSGetShaderResources(0, 1, 0);

	

	this->gDeviceContext->IASetInputLayout(gVertexLayout);


	//Render all the models
	for (int i = 0; i < this->modelAmount; i++)
	{


		this->models->at(i)->render();


	}
	


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
			this->models->push_back(new Cube());
			this->modelAmount += 1;
			break;
		}

		case PLANE:
		{
			this->models->push_back(new Plane(this->gDevice, this->gDeviceContext));
			this->modelAmount += 1;
			break;
		}

		case PYRAMID:
		{
			this->models->push_back(new Pyramid(this->gDevice,this->gDeviceContext));
			this->modelAmount += 1;
			break;
		}

	}


}