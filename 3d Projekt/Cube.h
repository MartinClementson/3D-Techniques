#pragma once
#include "Model.h"
class Cube :
	public Model
{
private:
	void createVertices(ID3D11Device* gDevice);
	Cube();
public:
	Cube(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	Cube(const Cube &obj); //copy constructor
	~Cube();

	std::vector<Vertex>* getVerts();


	void update();
	
};

