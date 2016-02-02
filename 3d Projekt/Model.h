#pragma once
#include "Linker.h"
#include "ObjHandler.h"
using namespace DirectX;

class Model
{

protected:

	//Rotation bool, So every object can set rotation on/off
	bool rotate = false;


	XMFLOAT3 pivotPoint;

	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 normalWorld;

	std::vector<Vertex> *vertices;

	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Buffer* vertexBuffer = nullptr;

	worldConstantBuffer* worldStruct = nullptr;
	ID3D11Buffer* worldBuffer = nullptr;

	virtual void createVertices(ID3D11Device* gDevice);

	//Texture
	ID3D11ShaderResourceView* texture = nullptr; //If multitextures are to be supported. this will be an array
	ID3D11Texture2D *pTexture = nullptr;
	void loadTexture(ID3D11Device* gDevice, std::string filePath);
	Model();//Default constuctor
public:

	Model(ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	Model(const Model &obj); //copy constructor

	//This is a constructor for a OBJ file. this has a filePath string as a parameter
	Model(std::string filePath, ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct);
	virtual ~Model();
	
	virtual void update();
	virtual void render();

	virtual void updateWorldMatrix();

	//Setters

	void setRotateState(bool state);
	void setPivotPoint(XMFLOAT3 newPosition);

	void setTranslation(XMFLOAT3 newTranslation);
	void setRotation(XMFLOAT3 degrees);
	void setScale(XMFLOAT3 newScale);
	
	//Getters
	
	XMFLOAT3 getPivotPoint();

	XMFLOAT3 getTranslation();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale;

};

