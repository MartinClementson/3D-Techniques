#include "Model.h"



Model::Model()
{

	//this->vertices = new Vertex[3]{
	//	//First Tris
	//	-0.5f, 0.5f, 0.0f,	//v0 pos														
	//	 0.0f, 1.0f, 0.0f,

	//	 0.5f, 0.5f, 0.0f,	//v1
	//	 0.0f, 0.0f, 1.0f,

	//	-0.5f,-0.5f, 0.0f, //v2
	//	 1.0f, 0.0f, 0.0f,
	//};


}

Vertex* Model::getVerts()
{

	return this->vertices;
}


Model::~Model()
{

	delete[] vertices;
}


