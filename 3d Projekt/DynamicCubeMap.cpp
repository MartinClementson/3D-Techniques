#include "DynamicCubeMap.h"

/*
	For desciption. Read DynamicCubeMap.h
*/

DynamicCubeMap::DynamicCubeMap()
{
}


DynamicCubeMap::~DynamicCubeMap()
{
}





void DynamicCubeMap::Release()
{

	dynamicCubemapDSV->Release();
	for (int i = 0; i < 6; i++)
	{
		dynamicCubeMapRTV[i]->Release();
		delete cubeMapCamera[i];

	};

	cubeTex->Release();
	dynamicCubeMapResource->Release();
	depthTex->Release();
	

}

void DynamicCubeMap::Render(XMFLOAT3 modelPosition,Engine* engine) // We include engine, so that we can draw the scene on each view
{


	//Remove the cubemap as a resource in the pixel shader
	//This is needed so that we can render to it.

	ID3D11ShaderResourceView * tab[1];
	tab[0] = NULL;
	gDeviceContext->PSSetShaderResources(3, 1, tab);
	
	
	
	
	ID3D11RenderTargetView* renderTargets[1];

	this->BuildCubeFaceCamera(modelPosition);
	//set the viewPort to the cube map
	gDeviceContext->RSSetViewports(1, &cubeMapViewPort);
	float clearColor[] = { 0, 1, 0, 1 };
	//Render the scene for each face
	for (int i = 0; i < 6; i++)
	{
		//clear cube map face and depth buffer
		gDeviceContext->ClearRenderTargetView(dynamicCubeMapRTV[i], clearColor);
		gDeviceContext->ClearDepthStencilView(dynamicCubemapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//Bind cube map face as render target

		renderTargets[0] = dynamicCubeMapRTV[i];
		gDeviceContext->OMSetRenderTargets(1, renderTargets, dynamicCubemapDSV);

		//Draw the scene with the exception of the object the camera is in
		//(needs to be implemented)
		engine->updateCamera(this->cubeMapCamera[i]);
		engine->renderScene();

	}

	//Restore old viewPort and renderTargets
	gDeviceContext->RSSetViewports(1, &engine->getViewPort());

	renderTargets[0] = engine->getRenderTargetView();
	
	gDeviceContext->OMSetRenderTargets(1, renderTargets, engine->getDepthStencilView());

	//Generate lower mipmap levels of the cube map
	gDeviceContext->GenerateMips(dynamicCubeMapResource);

	//Set the cubemap as a resource
	gDeviceContext->PSSetShaderResources(3, 1, &dynamicCubeMapResource);


}

void DynamicCubeMap::Update()
{
}

bool DynamicCubeMap::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	HRESULT hr;

	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;

	for (int i = 0; i < 6; i++)
	{
		cubeMapCamera[i] = new Camera();
	}

	//Create the cube map texture:

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = CubeMapSize;
	texDesc.Height = CubeMapSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = gDevice->CreateTexture2D(&texDesc, 0, &cubeTex);
	if (FAILED(hr))
		return false;

	//Create all the render targetVievs
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;

	//This is one, because we will only create the view for one element at the time
	rtvDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; i++)
	{
		//Create the six rendertargets
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		hr = gDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &dynamicCubeMapRTV[i]);
		if (FAILED(hr))
			return false;


	}

	//Create a shader resource view to the cube map


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	hr = gDevice->CreateShaderResourceView(cubeTex, &srvDesc, &dynamicCubeMapResource);
	if (FAILED(hr))
		return false;
	
	//View Saves reference
	//cubeTex->Release();

	//Set up the depthBuffer

	D3D11_TEXTURE2D_DESC depthDesc;

	depthDesc.Width = CubeMapSize;
	depthDesc.Height = CubeMapSize;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	hr = gDevice->CreateTexture2D(&depthDesc, NULL, &depthTex);

	if (FAILED(hr))
		return false;

	//Create the depth stencil view

	D3D11_DEPTH_STENCIL_VIEW_DESC dvsDesc;
	dvsDesc.Format = depthDesc.Format;
	dvsDesc.Flags = 0;
	dvsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dvsDesc.Texture2D.MipSlice = 0;

	hr = gDevice->CreateDepthStencilView(depthTex, &dvsDesc, &dynamicCubemapDSV);

	if (FAILED(hr))
		return false;
	//View Saves reference
	//depthTex->Release();

	//set the viewPort

	cubeMapViewPort.TopLeftX = 0;
	cubeMapViewPort.TopLeftY = 0;
	cubeMapViewPort.Width = (float)CubeMapSize;
	cubeMapViewPort.Height = (float)CubeMapSize;
	cubeMapViewPort.MinDepth = 0.0f;
	cubeMapViewPort.MaxDepth = 1.0f;



	return true;
}

void DynamicCubeMap::BuildCubeFaceCamera(XMFLOAT3 position)
{
	//Look along each coordinate axis

	XMFLOAT3 targets[6] =
	{
		XMFLOAT3(position.x + 1.0f, position.y, position.z), // +X
		XMFLOAT3(position.x - 1.0f, position.y, position.z), // -X
		XMFLOAT3(position.x, position.y + 1.0f, position.z), // +Y
		XMFLOAT3(position.x, position.y - 1.0f, position.z), // -Y
		XMFLOAT3(position.x, position.y, position.z + 1.0f), // +Z
		XMFLOAT3(position.x, position.y, position.z - 1.0f) // -Z

	};

	/* use world up vector (0,1,0) for all directions except +Y/-Y, In these cases we need a different up vector*/
	XMFLOAT3 ups[6] =
	{
		XMFLOAT3(0.0f, 1.0f, 0.0f), // +X
		XMFLOAT3(0.0f, 1.0f, 0.0f), // -X
		XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
		XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
		XMFLOAT3(0.0f, 1.0f, 0.0f), // +Z
		XMFLOAT3(0.0f, 1.0f, 0.0f) // -Z

	};

	for (int i = 0; i < 6; i++)
	{
		
		cubeMapCamera[i]->setViewLookAt(targets[i]);
		cubeMapCamera[i]->setViewPosition(position);
		cubeMapCamera[i]->setViewUpDirection(ups[i]);
		cubeMapCamera[i]->setCameraLens(0.5f*PI, 1.0f, 0.1f, 1000.0f);
		cubeMapCamera[i]->updateView();


	}

}
