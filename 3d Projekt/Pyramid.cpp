#include "Pyramid.h"



void Pyramid::createVertices()
{
	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, PAD,
		1.0f, 1.0f, 1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		
		-0.5f, -0.5f , 0.5f, PAD,
		0.0f, 0.0f,   1.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});


	//second tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, PAD,
		1.0f, 1.0f, 1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , 0.5f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , -0.5f, PAD,
		1.0f, 0.0f,   0.0f, PAD
	});


	//third tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, PAD,
		1.0f, 1.0f, 1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f, PAD,
		1.0f, 0.0f,   0.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   1.0f, PAD
	});


	//fourth tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, PAD,
		1.0f, 1.0f, 1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		// Vert
		-0.5f, -0.5f , -0.5f, PAD,
		0.0f, 1.0f,   1.0f, PAD
	});
	this->vertices->push_back(Vertex
	{
		// Vert
		-0.5f, -0.5f , 0.5f, PAD,
		0.0f, 0.0f,   1.0f, PAD
	});


	//Bottom quad

	//first tris
	
	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f, PAD,
		1.0f, 1.0f, 1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f, PAD,
		1.0f, 1.0f,   1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , -0.5f, PAD,
		1.0f, 1.0f,   1.0f, PAD
	});

	//Second tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, -0.5f , 0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f, PAD,
		1.0f, 1.0f,   1.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , 0.5f, PAD,
		1.0f, 1.0f,   1.0f, PAD
	});

}

Pyramid::Pyramid()
{
	this->vertices = new std::vector<Vertex>;
	this->createVertices();
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

std::vector<Vertex>* Pyramid::getVerts()
{
	return this->vertices;
}
