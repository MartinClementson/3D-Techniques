#include "ComputeShaderClass.h"



ComputeShaderClass::ComputeShaderClass()
{
}


ComputeShaderClass::~ComputeShaderClass()
{
}

bool ComputeShaderClass::Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11UnorderedAccessView*	uav)
{

	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	this->uav = uav;
	HRESULT hr;
	//Connecting the CS
	ID3DBlob *pCs = nullptr;
	D3DCompileFromFile(
		L"ComputeShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"cs_5_0",
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
	ID3D11UnorderedAccessView* uavA[] = { uav };

	gDeviceContext->CSSetUnorderedAccessViews(0, 1, uavA, nullptr);
	gDeviceContext->CSSetShader(this->gCompute, nullptr, 0);

	gDeviceContext->Dispatch(32, 30, 1);

	uavA[0] = nullptr;
	gDeviceContext->CSSetShader(nullptr, nullptr, 0);
	gDeviceContext->CSSetUnorderedAccessViews(0, 1, uavA, nullptr);


		


}
