#pragma once

struct Vertex 
{
	//sizeof(float) == 4 byte!!!

	float x, y, z; // 0 byte offset
	
	float r, g, b; //12 byte offset

	float u, v; // 24 byte offset

	float nx, ny, nz; // 32 byte offset


	struct vertexTangent { // 44 byte offset
		float x, y, z;
		vertexTangent& operator+(const vertexTangent& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
	};
	vertexTangent tangent;

	std::vector<vertexTangent> sharedTangents;
	void interpolateTangents()
	{
		vertexTangent temp;
		temp.x = 0.0f;
		temp.y = 0.0f;
		temp.z = 0.0f;
		for (int i = 0; i < sharedTangents.size(); i++)
			temp = temp + sharedTangents.at(i);
		tangent.x = temp.x / sharedTangents.size();
		tangent.y = temp.y / sharedTangents.size();
		tangent.z = temp.z / sharedTangents.size();
		sharedTangents.clear();
	
	}

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