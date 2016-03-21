#include "ComputeShaderClass.h"



ComputeShaderClass::ComputeShaderClass()
{
}


ComputeShaderClass::~ComputeShaderClass()
{
}

bool ComputeShaderClass::Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	HRESULT hr;
	//Connecting the CS
	ID3DBlob *pCs = nullptr;
	D3DCompileFromFile(
		L"ComputeShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"cs_4_0",
		0,
		0,
		&pCs,
		nullptr);

	hr = gDevice->CreateComputeShader(pCs->GetBufferPointer(),
		pCs->GetBufferSize(), NULL, &gCompute);

	if (FAILED(hr))
		return false;

	pCs->Release();
	return true;
}

void ComputeShaderClass::Release()
{
	this->gCompute->Release();
}

void ComputeShaderClass::Dispatch()
{



}
