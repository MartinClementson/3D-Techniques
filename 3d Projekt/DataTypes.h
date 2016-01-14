#pragma once

struct Vertex {

	float x, y, z, pad;
	
	float r, g, b, ColorPad; //change this to UV later
	
};

struct worldViewProjection
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

