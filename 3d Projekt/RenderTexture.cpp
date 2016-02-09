#include "RenderTexture.h"



RenderTexture::RenderTexture()
{
	
}


RenderTexture::~RenderTexture()
{
}


bool RenderTexture::Init(ID3D11Device* device, int textureWidth, int textureHeight)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;


	ZeroMemory(&textureDesc, sizeof(textureDesc));
	//Set up the render texture desciption

	textureDesc.Width = textureWidth/4;
	textureDesc.Height = textureHeight/4;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Create the render target Texture

	hr = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
	if (FAILED(hr))
		return false;



	//set up description for render target view
	renderTargetDesc.Format = textureDesc.Format;
	renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 0;

	//Create render target
	
	hr = device->CreateRenderTargetView(renderTargetTexture, &renderTargetDesc, &renderTargetView);
	if (FAILED(hr))
		return false;

	//Set up the shader resource view

	resourceViewDesc.Format = textureDesc.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 1;

	//Create the resourceView;

	hr = device->CreateShaderResourceView(renderTargetTexture, &resourceViewDesc, &shaderResourceView);
	if (FAILED(hr))
		return false;
	return true;

}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	return;
}
void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
	float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	//Clear backbuffer
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	//clearDepthBuffer
	//deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//Might not be needed when only using one camera, 
	return;
}

ID3D11ShaderResourceView * RenderTexture::GetShaderResourceView()
{
	return this->shaderResourceView;
}

void RenderTexture::Release()
{

	renderTargetTexture->Release();
	renderTargetView->Release();
	shaderResourceView->Release();

}