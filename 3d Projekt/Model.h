#pragma once
#include "Linker.h"

using namespace DirectX;

class Model
{

protected:

#pragma region Model attributes
	//Rotation bool, So that every object can set rotation on/off
	bool rotate = false;
	float rotationSpeed = 0.2f;


	XMFLOAT3 pivotPoint = { 0.0f,0.0f,0.0f};
	XMFLOAT3 translation = { 0.0f,0.0f,0.0f};
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 normalWorld;

#pragma endregion

#pragma region COM objects and Class objects
	std::vector<Model*>* children = nullptr; //A model can have other models as it's children

	std::vector<Vertex> *vertices = nullptr;
	UINT indicesCount = 0;
	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	worldConstantBuffer* worldStruct = nullptr;
	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine
	
	pixelShaderConstants renderState; // Every Object has it's own renderstate.
	/* 
	For example. Some Models have normal maps, some don't.
	We query
	*/


	materialConstBuffer materialValues; // This will hold each objects mtl information from the obj importer.
	//ID3D11Buffer* materialBuffer = nullptr; //This is a pointer to the material const buffer. updated each object.


										 //Texture
	ID3D11ShaderResourceView* texture = nullptr; //If multitextures are to be supported. this will be an array
	ID3D11ShaderResourceView* normalMap = nullptr;
#pragma endregion

#pragma region Private functions
	void calculateVertVectors();
	void calculateTangentBinormal(Vertex vertex1, Vertex vertex2, Vertex vertex3, DirectX::XMFLOAT3 &tangent, DirectX::XMFLOAT3 &biNormal);
	void sendToConstantBuffer();
	
	virtual void createVertices(ID3D11Device* gDevice);
	void loadTexture(ID3D11Device* gDevice, std::string filePath);
#pragma endregion

public:

#pragma region Constructors
	Model();//Default constuctor


	//this is the constructor for the children in the obj importer
	Model(std::vector<Vertex>* vertArray, std::string* texturePath, ID3D11Device* gDevice,
		ID3D11DeviceContext * gDeviceContext, ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, std::vector<UINT> &indices);


	//This is the constructor for the primitives
	Model(ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	
	Model(const Model &obj); //copy constructor
	
	 //This is a constructor for a OBJ file. this has a filePath string as a parameter
	Model(std::string filePath, ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	
	virtual ~Model();
#pragma endregion (Destructor included)
	
	
#pragma region Public functions
	virtual void update();
	virtual void render(modelBuffers* objectInfo);
	void Release();
	void loadNormal(ID3D11Device* gDevice, std::string filePath);
	void renderChildren(modelBuffers* objectInfo);

	virtual void updateWorldMatrix();
#pragma endregion


#pragma region Setters
	//Setters

	void setVertex(Vertex nVertex); //<---------------------- may not be needed
	void setRotateState(bool state);
	void setRotationSpeed(float speed) { this->rotationSpeed = speed; };
	void setPivotPoint(XMFLOAT3 newPosition);

	void setTranslation(XMFLOAT3 newTranslation);
	void setRotation(XMFLOAT3 degrees);
	void setScale(XMFLOAT3 newScale);
#pragma endregion
	
	
#pragma region Getters
	//Getters
	
	XMFLOAT3 getPivotPoint();

	XMFLOAT3 getTranslation();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale;
#pragma endregion



};

