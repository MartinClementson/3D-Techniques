#include "Model.h"



Model::Model()
{

	this->vertices = new Vertex[3]{

		//First Tris
		0.0f, 0.5f, 0.0f, PAD,	//v0 pos														
		1.0f, 0.0f, 0.0f, PAD,

		0.5f, -0.5f, 0.0f,PAD,	//v1
		0.0f,  1.0f, 0.0f,PAD,

	   -0.5f, -0.5f, 0.0f,PAD, //v2
		0.0f,  0.0f, 1.0f,PAD
	};


}

Vertex* Model::getVerts()
{

	return this->vertices;
}


Model::~Model()
{

	delete[] vertices;
}


