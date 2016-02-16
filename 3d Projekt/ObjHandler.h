#pragma once
#include "Linker.h"
#include "Model.h"
class Model;
class ObjHandler
{

private:

public:
	ObjHandler();
	
	ObjHandler(std::vector<Model*>* childrenArray,std::string filePath, std::vector<Vertex>* modelVerts,
		std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
		ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct);

	void create(std::vector<Model*>* childrenArray, std::vector<Vertex>* modelVerts,
		std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
		ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, int &count, vector<DirectX::XMFLOAT3> uvCoord,
		vector<DirectX::XMFLOAT3> vCoord, vector<DirectX::XMINT3> testIn, int *offset, bool *father);

	std::string MtlHandler(std::string &filePath, std::string &material); //change this later to be able to include all the material components

	~ObjHandler();
};

