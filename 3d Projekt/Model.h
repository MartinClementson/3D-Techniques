#pragma once
#include "Linker.h"
using namespace DirectX;

class Model
{

protected:
	XMFLOAT3 pivotPoint;

	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	std::vector<Vertex> *vertices;

	virtual void createVertices();

public:
	Model();//Default constuctor
	Model(const Model &obj); //copy constructor
	virtual ~Model();
	

	//Setters
	void setPivotPoint(XMFLOAT3 newPosition);

	//Getters
	virtual std::vector<Vertex>* getVerts();
	XMFLOAT3 getPivotPoint();

};

