#include "Pyramid.h"



void Pyramid::createVertices(ID3D11Device* gDevice)
{
	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f,
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		
		-0.5f, -0.5f , 0.5f,
		0.0f, 0.0f,   1.0f
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f
	});


	//second tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, 
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , 0.5f, 
		0.0f, 1.0f,   0.0f
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , -0.5f,
		1.0f, 0.0f,   0.0f
	});


	//third tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, 
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f,
		1.0f, 0.0f,   0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, 
		0.0f, 1.0f,   1.0f
	});


	//fourth tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f,
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		// Vert
		-0.5f, -0.5f , -0.5f, 
		0.0f, 1.0f,   1.0f
	});
	this->vertices->push_back(Vertex
	{
		// Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 0.0f,   1.0f
	});


	//Bottom quad

	//first tris
	
	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f, 
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		
		-0.5f, -0.5f , -0.5f,
		1.0f, 1.0f,   1.0f
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , -0.5f, 
		1.0f, 1.0f,   1.0f
	});


	//Second tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, -0.5f , 0.5f, 
		1.0f, 0.0f, 0.0f
	});

	this->vertices->push_back(Vertex
	{
		-0.5f, -0.5f , 0.5f,
		1.0f, 1.0f,   1.0f
	});

	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , -0.5f,
		1.0f, 1.0f,   1.0f
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

Pyramid::Pyramid()
{
	this->vertices = new std::vector<Vertex>;
	//this->createVertices();
}

Pyramid::Pyramid(ID3D11Device* gDevice,ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct)
	: Model(gDeviceContext, worldBuffer, worldStruct)
{


	
	this->createVertices(gDevice);
}

Pyramid::Pyramid(const Pyramid & obj)
{

	this->pivotPoint = obj.pivotPoint;
	this->translation = obj.translation;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->vertices = new std::vector<Vertex>;
	for (size_t i = 0; i < obj.vertices->size(); i++)
	{
		this->vertices->push_back(obj.vertices->at(i));
	}

}


Pyramid::~Pyramid()
{
	

}

void Pyramid::update()
{
	
	
	
	
	return Model::update();
	
}

void Pyramid::render()
{
	return Model::render();

}

