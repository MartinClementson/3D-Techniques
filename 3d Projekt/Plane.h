#pragma once
#include "Model.h"
class Plane :public Model
{
private:
	void createVertices();
public:
	Plane();

	virtual ~Plane();

	std::vector<Vertex>* getVerts();

	Plane(const Plane &obj); //copy constructor
};

