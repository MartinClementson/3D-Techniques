#pragma once
struct position {

	float x, y, z;
};

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

	std::vector<vertexTangent> *sharedTangents = nullptr;

	void createTangent()
	{
		sharedTangents = new std::vector<vertexTangent>;
	}

	//some vertices recieve more than one tangents (because that specific vertex
	//is connected to more than one face), because of that we interpolate between
	//them for a smother look on the resulting normal map
	void interpolateTangents()
	{
		vertexTangent temp;
		temp.x = 0.0f;
		temp.y = 0.0f;
		temp.z = 0.0f;
		for (int i = 0; i < sharedTangents->size(); i++)
			temp = temp + sharedTangents->at(i);
		tangent.x = temp.x / sharedTangents->size();
		tangent.y = temp.y / sharedTangents->size();
		tangent.z = temp.z / sharedTangents->size();
		sharedTangents->clear();
		if (sharedTangents != nullptr)
		{
			delete sharedTangents;
			sharedTangents = nullptr;
		}
	}
	~Vertex()
	{
		if (sharedTangents != nullptr)
		{
			delete sharedTangents;
			sharedTangents = nullptr;
		}

	}


	Vertex(float x,float y,float z,float r,float g,float b) {
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;
	};
	Vertex(float x, float y, float z, float r, float g, float b,float u, float v) {
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;

		this->u = u;
		this->v = v;
	};

	Vertex() {
		this->x = 0;
		this->y = 0;
		this->z = 0;

		this->r = 1;
		this->g = 1;
		this->b = 1;
	};

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






struct AnimVertex {

	//sizeof(float) == 4 byte!!!

	position pos; // 0 byte offset

	float u, v;  

	position normal; 

	int StartWeight; // won't be sent to shader
	int WeightCount; // won't be sent to shader


};


struct Joint {

	std::wstring name;
	int parentID;

	position pos;

	struct orientation {

		float x, y, z, w;
	};

	orientation orientation;

};

struct Weight
{
	int jointID;
	float influence;
	position pos;
	position normal;

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


	BOOL normalMap;   // 4 bytes
	
	BOOL distanceFog; // 4 bytes

	BOOL Padding[2];	  // 8 bytes


	pixelShaderConstants(BOOL normalMap, BOOL fog, BOOL miniMap) //Remove minimap
	{
		
		this->normalMap = normalMap;
		this->distanceFog = fog;
	};

	pixelShaderConstants()
	{
		
		this->normalMap = FALSE;
		this->distanceFog = TRUE;
	};

	bool operator==(const pixelShaderConstants& other)
	{
		if (
			this->distanceFog == other.distanceFog &&
			this->normalMap == other.normalMap)
			return true;
		else
			return false;

	};

	pixelShaderConstants& operator=(const pixelShaderConstants& other) //operator = overload
	{
		this->distanceFog = other.distanceFog;
		this->normalMap = other.normalMap;
		return *this;
	}

};