#pragma once
#include "Model.h"
class Pyramid :
	public Model
{

private:

	void createVertices(ID3D11Device* gDevice);
	Pyramid();
public:
	Pyramid(ID3D11Device* gDevice,ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	Pyramid(const Pyramid &obj);
	virtual ~Pyramid();

	void update();
	void render();

	std::vector<Vertex>* getVerts();
};

