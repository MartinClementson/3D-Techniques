#include "Cube.h"



Cube::Cube()
{
	this->vertices = new std::vector<Vertex>;
	createVertices();
}


Cube::~Cube()
{
}

void Cube::createVertices()
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
}

std::vector<Vertex>* Cube::getVerts()
{
	return this->vertices;
}