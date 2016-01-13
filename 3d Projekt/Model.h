#pragma once
#include "Linker.h"
class Model
{

protected:

	Vertex* vertices;


public:
	Model();
	virtual ~Model();
	
	Vertex* getVerts();

};

