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
	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Buffer* vertexBuffer = nullptr;
	virtual void createVertices(ID3D11Device* gDevice);

public:
	Model();//Default constuctor
	Model(const Model &obj); //copy constructor
	virtual ~Model();
	
	virtual void update();
	virtual void render();

	//Setters
	void setPivotPoint(XMFLOAT3 newPosition);

	//Getters
	virtual std::vector<Vertex>* getVerts();
	XMFLOAT3 getPivotPoint();

};

