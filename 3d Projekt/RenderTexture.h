#pragma once
#include "Linker.h"

#pragma region Comment
//pragma region makes the text collapsable
/*
	
	 Author: Martin


	What this class does: it creates a render target and store what is rendered
	to a texture. 

	When this is used. the scene will not be rendered to the backbuffer.
	therefore, the user wont see what is rendered as it is only stored
	as a texture.

	To use this and still let the user see the scene, this needs to be a separate
	rendering pass.

	The texture can be used (after it's render pass) as any other texture.
	Making it a dynamic texture.

	This is, connected to the player camera. But can be set to any camera.


	This is the fundamental technique to many things such as shadow mapping,
	dynamic cube mapping, projective texturing, and more.


	This technique is not on the list of things to do in the project.
	However, it is very good to know how this works before turning to 
	more advanced variations of this

*/
#pragma endregion Description of class




class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();
	bool Init(ID3D11Device* device, int textureWidth, int textureHeight);
	void Release();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	ID3D11RenderTargetView** getRenderTarget() { return &this->renderTargetView; };
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
		float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* renderTargetTexture =nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
};

