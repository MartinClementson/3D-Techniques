#include "Model.h"



Model::Model()
{

	this->vertices = new Vertex[3];

	this->vertices[0] = {
		//First Tris
		0.0f, 0.5f, 0.0f, PAD,	//v0 pos														
		1.0f, 0.0f, 0.0f, PAD };

	this->vertices[1] = {
		//second Tris
		0.5f, -0.5f, 0.0f,PAD,	//v1
		0.0f,  1.0f, 0.0f,PAD };

	this->vertices[2] = {
		//Third Tris

	   -0.5f, -0.5f, 0.0f,PAD, //v2
		0.0f,  0.0f, 1.0f,PAD };
	


}


void Model::setPivotPoint(XMFLOAT3 newPosition)
{

	this->pivotPoint = newPosition;

}

XMFLOAT3 Model::getPivotPoint()
{
	return this->pivotPoint;

}


Vertex* Model::getVerts()
{

	return this->vertices;
}


Model::~Model()
{

	delete[] vertices;
}


