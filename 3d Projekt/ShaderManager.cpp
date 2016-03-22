#include "ShaderManager.h"


//Read ShaderManager.h for description

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

void ShaderManager::Release()
{
	//Release Color shaders
	gVertexLayoutColor->Release();
	gVertexShaderColor->Release();
	gPixelShaderColor->Release();
	gGeometryShaderColor->Release();

	//Release Texture shaders

	gVertexLayoutTexture->Release(); //If this crashes on shut down, is because there is no layout for texture yet

	gVertexShaderTexture->Release();
	gPixelShaderTexture->Release();
	gGeometryShaderTexture->Release();

	//Release Skymap shaders
	if (SKYMAP_VS != nullptr)
		SKYMAP_VS->Release();
	if (SKYMAP_GS != nullptr)
		SKYMAP_GS->Release();
	if (SKYMAP_PS != nullptr)
		SKYMAP_PS->Release();

	gVertexLayoutSky->Release();

	gSampleState->Release();


	CUBEMAP_VS->Release();
	CUBEMAP_GS->Release();
	CUBEMAP_PS->Release();
	gVertexLayoutCubeMap->Release();



	TERRAIN_VS->Release();
	TERRAIN_GS->Release();
	TERRAIN_PS->Release();
	gVertexLayoutTerrain->Release();

	OVERLAY_VS->Release();
	OVERLAY_GS->Release();
	OVERLAY_PS->Release();
	gVertexLayoutOverlay->Release();


	ANIMATION_VS->Release();
	ANIMATION_GS->Release();
	ANIMATION_PS->Release();
	gVertexLayoutAnimation->Release();





}

bool ShaderManager::createShaders()
{
	if (!createColorShader())
		return false;
	if (!createTextureShader())
		return false;
	if (!createSkyShader())
		return false;
	if (!createCubeMapShader())
		return false;
	if (!createTerrainShader())
		return false;
	if (!createOverlayShader())
		return false;
	if (!createAnimationShader())
		return false;


	return true;
}

bool ShaderManager::createTextureShader()
{
	//Create a sample state first



	D3D11_SAMPLER_DESC samplerDesc;
	// use linear interpolation for minification, magnification, and mip-level sampling (quite expensive)
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	//for all filters: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476132(v=vs.85).aspx

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap, (repeat) for use of tiling texutures
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f; //mipmap offset level
	samplerDesc.MaxAnisotropy = 1; //Clamping value used if D3D11_FILTER_ANISOTROPIC or D3D11_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values are between 1 and 16.
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0; //0 most detailed mipmap level, higher number == less detail
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	hr = gDevice->CreateSamplerState(&samplerDesc, &gSampleState);
	

	if (FAILED(hr))
	{
		return false;
		

	}
	else
	{
		//Set sampler to pixel shader and the compute shader
		gDeviceContext->PSSetSamplers(0, 1, &this->gSampleState);
		gDeviceContext->CSSetSamplers(0, 1, &this->gSampleState);
	}
		
	//Load the shaders



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

	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMALS",0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 32,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//We wont use Color here, that's why the offset is 32. were still using the same struct
		{ "TANGENT",0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 44,D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	if (FAILED(hr))
		return false;


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

	if (FAILED(hr))
		return false;

	return true;
	
}

bool ShaderManager::createColorShader()
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
	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 }

	};

	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutColor);
	pVS->Release();
	if (FAILED(hr))
		return false;

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

	if (FAILED(hr))
		return false;

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
	if (FAILED(hr))
		return false;

	return true;
}

bool ShaderManager::createSkyShader()
{
	HRESULT hr;
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderSky.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &SKYMAP_VS);

	if (FAILED(hr))
		return false;


	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0 } //We wont use Color here, that's why the offset is 32. were still using the same struct
																						//Normals?

	};
	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutSky);
	pVS->Release();
	if (FAILED(hr))
		return false;

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderSky.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &SKYMAP_GS);
	pGS->Release();

	if (FAILED(hr))
		return false;









	//Pixel shader
	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderSky.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &SKYMAP_PS);
	pPs->Release();
	if (FAILED(hr))
		return false;








	return true;
}

bool ShaderManager::createTerrainShader()
{

	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderTerrain.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &TERRAIN_VS);

	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0 } //We wont use Color here, that's why the offset is 32. were still using the same struct
																						//Normals?

	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutTerrain);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderTerrain.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &TERRAIN_PS);
	pPs->Release();

	if (FAILED(hr))
		return false;


	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderTerrain.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &TERRAIN_GS);
	pGS->Release();

	if (FAILED(hr))
		return false;

	return true;
}

bool ShaderManager::createCubeMapShader()
{
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderCubeMap.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &CUBEMAP_VS);

	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0 } //We wont use Color here, that's why the offset is 32. were still using the same struct
																						//Normals?

	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutCubeMap);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderCubeMap.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &CUBEMAP_PS);
	pPs->Release();

	if (FAILED(hr))
		return false;


	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderCubeMap.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &CUBEMAP_GS);
	pGS->Release();

	if (FAILED(hr))
		return false;

	return true;
}


bool ShaderManager::createOverlayShader()
{
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderOverlay.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &OVERLAY_VS);
	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 }

	};

	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutOverlay);
	pVS->Release();
	if (FAILED(hr))
		return false;

	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderOverlay.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &OVERLAY_PS);
	pPs->Release();

	if (FAILED(hr))
		return false;

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderOverlay.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &OVERLAY_GS);
	pGS->Release();
	if (FAILED(hr))
		return false;

	return true;
}

bool ShaderManager::createAnimationShader()
{

	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderAnimation.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &ANIMATION_VS);

	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	// We are using the AnimVertex Struct here
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
	
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(position), D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMALS",0, DXGI_FORMAT_R32G32B32_FLOAT , 0, sizeof(position)+ 8,D3D11_INPUT_PER_VERTEX_DATA, 0 }
		
	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutAnimation);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderAnimation.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &ANIMATION_PS);
	pPs->Release();

	if (FAILED(hr))
		return false;


	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderAnimation.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &ANIMATION_GS);
	pGS->Release();

	if (FAILED(hr))
		return false;

	return true;



}


bool ShaderManager::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;

	if (!createShaders())
		return false;

	return true;
}



void ShaderManager::setActiveShaders(ShaderTypes shader) {

	switch (shader)
	{
	case TEXTURESHADER:
		this->gDeviceContext->VSSetShader(gVertexShaderTexture, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(gGeometryShaderTexture, nullptr, 0);
		this->gDeviceContext->PSSetShader(gPixelShaderTexture, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutTexture);
		break;

	case COLORSHADER:
		this->gDeviceContext->VSSetShader(gVertexShaderColor, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(gGeometryShaderColor, nullptr, 0);
		this->gDeviceContext->PSSetShader(gPixelShaderColor, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutColor);
		break;

	case SKYBOXSHADER:
		this->gDeviceContext->VSSetShader(SKYMAP_VS, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(SKYMAP_GS, nullptr, 0);
		this->gDeviceContext->PSSetShader(SKYMAP_PS, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutSky);
		break;

	case CUBEMAPSHADER:

		this->gDeviceContext->VSSetShader(CUBEMAP_VS, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(CUBEMAP_GS, nullptr, 0);
		this->gDeviceContext->PSSetShader(CUBEMAP_PS, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutCubeMap);
		break;

	case TERRAINSHADER:

		this->gDeviceContext->VSSetShader(TERRAIN_VS, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(TERRAIN_GS, nullptr, 0);
		this->gDeviceContext->PSSetShader(TERRAIN_PS, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutTerrain);
		break;


	case OVERLAYSHADER:

		this->gDeviceContext->VSSetShader(OVERLAY_VS, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(OVERLAY_GS, nullptr, 0);
		this->gDeviceContext->PSSetShader(OVERLAY_PS, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutOverlay);
		break;

	case ANIMATIONSHADER:

		this->gDeviceContext->VSSetShader(ANIMATION_VS, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(ANIMATION_GS, nullptr, 0);
		this->gDeviceContext->PSSetShader(ANIMATION_PS, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutAnimation);
		break;








	}


}
