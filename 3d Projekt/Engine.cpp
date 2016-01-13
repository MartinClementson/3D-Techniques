#include "Engine.h"



Engine::Engine()
{
}

Engine::Engine(HWND* winHandle)
{
	hr = CreateDirect3DContext(winHandle);

	setViewPort();

	/*models = new Model();*/ //this will be an array 

	//temporary, to be moved

	//D3D11_BUFFER_DESC bufferDesc;
	//Vertex* tempVerts = models->getVerts();
	//memset(&bufferDesc, 0, sizeof(bufferDesc));

	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(tempVerts); //kan vara fel

	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = tempVerts;
	//this->gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	//
	//newFunction();
	
}



Engine::~Engine()
{
	//delete models; //this needs to be an array delete <------------
	//Releasing everything
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gVertexBuffer->Release();
	//gGeometryShader->Release()  Geometry shader not in use yet
	gSwapChain->Release();
	gBackbufferRTV->Release();
	gDeviceContext->Release();
	gDevice->Release();


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
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0}
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

void Engine::newFunction()
{

	Vertex vertices[3]{
		//First Tris
		0.0f, 0.5f, 0.0f,	//v0 pos														
		1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.0f,	//v1
		0.0f, 1.0f, 0.0f,

		-0.5f,-0.5f, 0.0f, //v2
		0.0f, 0.0f, 1.0f,
	};
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices); //kan vara fel

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
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

	this->gDeviceContext->IAGetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->gDeviceContext->IASetInputLayout(gVertexLayout);

	this->gDeviceContext->Draw(3, 0); //This will be dynamic,



}