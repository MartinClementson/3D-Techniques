#pragma once
#include "Linker.h"
#include "Model.h"
class ObjHandler
{

private:

public:
	ObjHandler();
	
	ObjHandler(std::string filePath, std::vector<Vertex>* modelVerts, 
		std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
		ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct);

	std::string MtlHandler(std::string &filePath); //change this later to be able to include all the material components

	~ObjHandler();
};

