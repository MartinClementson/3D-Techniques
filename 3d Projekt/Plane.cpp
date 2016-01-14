#include "Plane.h"



Plane::Plane()
{

	this->vertices = new std::vector<Vertex>;
	createVertices();


}


void Plane::createVertices()
{

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.0f , -0.5f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.2f , 1.0f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.2f , 1.0f, PAD,
		0.0f, 0.0f,   1.0f, PAD
	});


}

Plane::~Plane()
{
	//delete[] vertices;
}

std::vector<Vertex>* Plane::getVerts()
{
	return this->vertices;
}

Plane::Plane(const Plane & obj) //Copy constructor
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
