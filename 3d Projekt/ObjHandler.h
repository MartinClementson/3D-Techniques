#pragma once
#include "Linker.h"
class ObjHandler
{

private:

public:
	ObjHandler();
	
	ObjHandler(std::string filePath, std::vector<Vertex>* modelVerts);

	void MtlHandler(std::string &filePath);

	~ObjHandler();
};

