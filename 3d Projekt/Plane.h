#pragma once
#include "Model.h"
class Plane :public Model
{
private:
	void createVertices(ID3D11Device* gDevice);
	Plane();
public:
	Plane(const Plane &obj); //copy constructor
	Plane(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	~Plane();




	void update();
	void render();

};

