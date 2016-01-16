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
};

