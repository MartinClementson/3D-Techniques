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


	Vertex* vertices;


public:
	Model();
	virtual ~Model();
	
	Vertex* getVerts();

	//Setters
	void setPivotPoint(XMFLOAT3 newPosition);

	//Getters
	XMFLOAT3 getPivotPoint();

};

