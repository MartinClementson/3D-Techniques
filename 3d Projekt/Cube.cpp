#include "Cube.h"



Cube::Cube()
{
	this->vertices = new std::vector<Vertex>;
	//createVertices();
}

Cube::Cube(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct)
	: Model(gDeviceContext, worldBuffer, worldStruct)
{
	

	this->createVertices(gDevice);
}

Cube::Cube(const Cube & obj)
{
}


Cube::~Cube()
{
}

void Cube::createVertices(ID3D11Device* gDevice)
{
	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , 0.5f, PAD,
		0.0f, 0.0f,   1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});



	//Second Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Third Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Fourth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Fifth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , -0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Sixth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , -0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Seventh Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , -0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Eighth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , -0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Ninth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, 0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Tenth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, 0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//Eleventh Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	//Twelfth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* vertices->size();


	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = vertices->data();
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."

	gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
}

std::vector<Vertex>* Cube::getVerts()
{
	return this->vertices;
}

void Cube::update()
{
	float static angle = 0; //<----- just temporary to test rotation
	angle += 0.01f;
	this->setRotation(XMFLOAT3(0, angle, 0));
	return Model::update();
}

void Cube::render()
{

	return Model::render();
}
