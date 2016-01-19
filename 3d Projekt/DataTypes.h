#pragma once

struct Vertex {

	float x, y, z, pad;
	
	float r, g, b, ColorPad; //change this to UV later
	
};

struct worldConstantBuffer
{
	DirectX::XMFLOAT4X4 world;
	
};

struct cameraConstantBuffer
{
	
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT3 camPos;
	float pad;
};

struct lightConstantBuffer
{
	DirectX::XMFLOAT4 lightPosition;
	DirectX::XMFLOAT4 lightColor;
	float intensity;
	DirectX::XMFLOAT3 pad;
};