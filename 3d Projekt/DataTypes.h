#pragma once

struct Vertex 
{
	//sizeof(float) == 4 byte!!!

	float x, y, z; // 0 byte offset
	
	float r, g, b; //12 byte offset

	float u, v; // 24 byte offset

	float nx, ny, nz; // 32 byte offset

	Vertex& operator=(const Vertex& other) //operator = overload
	{
		x = other.x;
		y = other.y;
		z = other.z;
		
		r = other.r;
		g = other.g;
		b = other.b;

		u = other.u;
		v = other.v;

		nx = other.nx;
		ny = other.ny;
		nz = other.nz;

		return *this;


	}

	
};


struct worldConstantBuffer
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 normalWorld;
};

struct cameraConstantBuffer
{
	
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT3 camLook;
	float pad;
	DirectX::XMFLOAT3 camPos;
	float pad2;
};

struct lightConstantBuffer
{
	DirectX::XMFLOAT4 lightPosition;
	DirectX::XMFLOAT4 lightColor;
	float intensity;
	DirectX::XMFLOAT3 pad;
};