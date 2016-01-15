#pragma once
#include "Model.h"
class Cube :
	public Model
{
private:
	void createVertices();
public:
	Cube();
	~Cube();

	std::vector<Vertex>* getVerts();
};

