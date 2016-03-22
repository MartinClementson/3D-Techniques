#pragma once
#include "Linker.h"
class ComputeShaderClass
{


private:
	ID3D11ComputeShader* gCompute = nullptr;
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;
	ID3D11UnorderedAccessView*	uav = nullptr;

public:
	ComputeShaderClass();
	~ComputeShaderClass();

	bool Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11UnorderedAccessView*	uav );
	void Release();
	void Dispatch();
};

