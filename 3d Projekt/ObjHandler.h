#pragma once
#include "Linker.h"
class ObjHandler
{

private:

public:
	ObjHandler();
	
	ObjHandler(std::string filePath, std::vector<Vertex>* modelVerts, std::string &textureName);

	std::string MtlHandler(std::string &filePath); //change this later to be able to include all the material components

	~ObjHandler();
};

