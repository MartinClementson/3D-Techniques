#pragma once
#include "Linker.h"
#include "Model.h"

class ObjHandler
{

private:

public:
	ObjHandler();
	
	ObjHandler(std::string filePath, Model &model);

	void MtlHandler(std::string &filePath);

	~ObjHandler();
};

