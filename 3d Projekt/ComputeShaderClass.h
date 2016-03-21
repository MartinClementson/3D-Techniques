#pragma once
#include "Linker.h"
class ComputeShaderClass
{


private:
	ID3D11ComputeShader* gCompute = nullptr;

public:
	ComputeShaderClass();
	~ComputeShaderClass();

	bool Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void Release();
	void Dispatch();
};

