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
	bool operator==(const Vertex& other)
	{
		if (x == other.x &&
			y == other.y &&
			z == other.z &&
			u == other.u &&
			v == other.v)
			return true;
		else
			return false;
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

struct pixelShaderConstants //hlsl uses 4 byte bools, c++ bools are 1 byte //Every register in hlsl is 16 byte (four floats)
{ /*
	WINDOWs booleans use 4 bytes, so we will use them instead

	c++ : sizeof(bool) = 1.0
	WINDOWS : sizeof(BOOL) = 4.0

	The struct needs to be a multiple of 16 bytes

  */


	BOOL miniMap;     // 4 bytes

	BOOL normalMap;   // 4 bytes
	
	BOOL distanceFog; // 4 bytes

	BOOL Padding;	  // 4 bytes


	pixelShaderConstants(BOOL normalMap, BOOL fog, BOOL miniMap)
	{
		this->miniMap = miniMap;
		this->normalMap = normalMap;
		this->distanceFog = fog;
	};

	pixelShaderConstants()
	{
		this->miniMap = FALSE;
		this->normalMap = FALSE;
		this->distanceFog = TRUE;
	};

	bool operator==(const pixelShaderConstants& other)
	{
		if (this->miniMap == other.miniMap &&
			this->distanceFog == other.distanceFog &&
			this->normalMap == other.normalMap)
			return true;
		else
			return false;

	};

	pixelShaderConstants& operator=(const pixelShaderConstants& other) //operator = overload
	{
		this->distanceFog = other.distanceFog;
		this->miniMap = other.miniMap;
		this->normalMap = other.normalMap;
		return *this;
	}

};