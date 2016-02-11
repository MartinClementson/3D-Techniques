#pragma once
#include "Linker.h"
class ShaderManager
{

private:
	HRESULT hr;
	// pointers to the main device and context
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;


	//Shaders for color
	ID3D11VertexShader* gVertexShaderColor = nullptr;
	ID3D11GeometryShader* gGeometryShaderColor = nullptr;
	ID3D11PixelShader* gPixelShaderColor = nullptr;
	ID3D11InputLayout* gVertexLayoutColor = nullptr;

	//Shaders for Texture
	ID3D11VertexShader* gVertexShaderTexture = nullptr;
	ID3D11GeometryShader* gGeometryShaderTexture = nullptr;
	ID3D11PixelShader* gPixelShaderTexture = nullptr;
	ID3D11InputLayout* gVertexLayoutTexture = nullptr;


	//SamplerStates
	ID3D11SamplerState* gSampleState = nullptr;
	
	bool createShaders();

	bool createTextureShader();
	bool createColorShader();
	bool createSkyShader();
public:
	ShaderManager();
	~ShaderManager();
	void Release();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	

	void setActiveShaders(ShaderTypes shader);
};

