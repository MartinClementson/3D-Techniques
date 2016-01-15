#pragma once
#include "Model.h"
class Pyramid :
	public Model
{

private:

	void createVertices();
public:
	Pyramid();
	Pyramid(const Pyramid &obj);
	virtual ~Pyramid();


	std::vector<Vertex>* getVerts();
};

