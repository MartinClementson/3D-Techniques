#include "Model.h"




Model::Model()
{

	//this->vertices = new std::vector<Vertex>;

	//vertices->push_back(Vertex {
	//
	//	//First Vert
	//	0.0f, 0.5f, 0.0f, PAD,	//v0 pos														
	//	1.0f, 0.0f, 0.0f, PAD });

	//vertices->push_back(Vertex{
	//	//second Vert
	//	0.5f, -0.5f, 0.0f,PAD,	//v1
	//	0.0f,  1.0f, 0.0f,PAD });

	//vertices->push_back(Vertex{
	//	//Third Vert

	//   -0.5f, -0.5f, 0.0f,PAD, //v2
	//	0.0f,  0.0f, 1.0f,PAD });
	//


}

void Model::createVertices()
{
}
Model::Model(const Model &obj) //Copy Constructor
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


void Model::setPivotPoint(XMFLOAT3 newPosition)
{

	this->pivotPoint = newPosition;

}

XMFLOAT3 Model::getPivotPoint()
{
	return this->pivotPoint;

}


std::vector<Vertex>* Model::getVerts()
{

	return this->vertices;
}


Model::~Model()
{

	delete vertices;
}


