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

	//Load the models and get their vertices
	this->models = new std::vector<Model>; //this will be an array 
	
	this->verticesToRender = new std::vector<Vertex>;
	loadModels();
	loadVertices();

	
	
	
	
}



Engine::~Engine()
{
	delete models; 
	delete verticesToRender;
}

void Engine::release()
{
	
	gVertexBuffer->Release();
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	//gGeometryShader->Release()  Geometry shader not in use yet
	gBackbufferRTV->Release();
	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
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


	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackbufferRTV);
		pBackBuffer->Release();

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, NULL); //NULL switches to depthstencilView later


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

	this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

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

	this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShader);
	pPs->Release();
}

void Engine::loadModels()
{
	
	this->models->push_back(Model());

	this->modelAmount += 1;
	
}

void Engine::loadVertices()
{
	//Loop through every model we have loaded
	for (int j = 0; j < modelAmount; j++)
	{	
		//Get the vertices of that model
		std::vector<Vertex>* tempVerts = models->at(j).getVerts();

			
			int modelVerts = tempVerts->size(); //using "modelVerts" to avoid doing function call every loop
			
			this->vertexAmount += modelVerts; //At the verts to the global vertCounter
			
			//For every model, loop through the vertices
			for (int i = 0; i < modelVerts; i++)
			{
				//Put each vertex into the dynamic array	
				this->verticesToRender->push_back(tempVerts->at(i));

			}
	}




	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* vertexAmount; 
	

	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = verticesToRender->data() ;
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."
	
	this->gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);

	
}

void Engine::run()
{

	this->update();

	this->render();

	this->gSwapChain->Present(0, 0); //Change front and back buffer after rendering


}

void Engine::update()
{


}

void Engine::render()
{
	float clearColor[] = { 0, 0, 0, 1 };


	this->gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	this->gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	//this->gDeviceContext->PSGetShaderResources(0, 1, 0);

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;

	this->gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->gDeviceContext->IASetInputLayout(gVertexLayout);

	this->gDeviceContext->Draw(vertexAmount, 0); //This will be dynamic,



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
			this->models->push_back(Cube());
			this->modelAmount += 1;
			break;
		}

		case PLANE:
		{
			this->models->push_back(Plane());
			this->modelAmount += 1;
			break;
		}

		case PYRAMID:
		{
			this->models->push_back(Pyramid());
			this->modelAmount += 1;
			break;
		}

	}


}