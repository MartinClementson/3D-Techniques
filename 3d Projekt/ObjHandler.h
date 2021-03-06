#pragma once
#include "Linker.h"
#include "Model.h"
class Model;
class ObjHandler
{

private:
	void calculateOBJTangents(std::vector<Vertex> &vertices, std::vector<UINT> &indices);
public:
	ObjHandler();

	struct Material
	{
		std::string mtlName;
		std::string fileName;
		materialConstBuffer values;
	};
	
	ObjHandler(std::vector<Model*>** childrenArray,std::string filePath, std::vector<Vertex>* modelVerts,
		std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
		ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, std::vector<UINT> &indices,
		materialConstBuffer &parentMaterial);

	void create(std::vector<Model*>** childrenArray, std::vector<Vertex>* modelVerts,
		Material &material, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
		ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, int &count, std::vector<DirectX::XMFLOAT3> *uvCoord,
		std::vector<DirectX::XMFLOAT3> *vCoord, std::vector<DirectX::XMINT3> *testIn, int &offset, bool &father,
		std::vector<UINT> &indices, std::vector<DirectX::XMFLOAT3> *vNCoord);

	void MtlHandler(std::string &filePath, std::vector<Material> &objMaterials); //change this later to be able to include all the material components

	~ObjHandler();
};

