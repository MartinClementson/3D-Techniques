#pragma once
#include "Linker.h"
//http://www.braynzarsoft.net/viewtutorial/q16390-27-loading-an-md5-model
class md5Model
{


private:
	struct ModelSubset {
		/*
		Each subset of our model will get their own one of these structures.
		It will have it's own vertex and index buffer. This is because when we animate 
		we don't have to update a buffer for the whole mesh. only the subsets that have changed.
		*/

	int texArrayIndex;
	int numTriangles;

	std::vector<AnimVertex> vertices;
	std::vector<DWORD> indices; // 32-bit unsigned int.
	std::vector<Weight> weights;

	std::vector<DirectX::XMFLOAT3> positions;

	ID3D11Buffer* vertBuff = nullptr;
	ID3D11Buffer* indexBuff = nullptr;

	void Release() { 
		if(vertBuff !=nullptr)
			this->vertBuff->Release(); 
		if (indexBuff != nullptr)
			this->indexBuff->Release(); }
	};

	
	
	int numSubsets;
	int numJoints;

	std::vector<Joint> joints;
	std::vector<ModelSubset> subsets;


	std::vector<ID3D11ShaderResourceView*> modelTextures;
	std::vector<std::wstring> texFileNameArray;
	DirectX::XMFLOAT4X4 worldMatrix;
	
	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;



	worldConstantBuffer* worldStruct = nullptr;
	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine
	

	void sendToConstantBuffer();
public:
	md5Model();
	bool Init(ID3D11DeviceContext* context, ID3D11Device* gDevice,ID3D11Buffer* worldbuffer);
	bool loadModel(ID3D11Device* gDevice);
	~md5Model();

	void update();
	void render();

	void Release();
};

