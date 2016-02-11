#pragma once
#include "Linker.h"
using namespace DirectX;
class SkyBox
{

private:


	//Pointer to the device and context
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext =nullptr;

	//The skyBox will need its own shaders, but we keep the cbuffers the same, to avoid making more constant buffers
	ID3D11VertexShader* SKYMAP_VS = nullptr;
	ID3D11GeometryShader* SKYMAP_GS = nullptr;
	ID3D11PixelShader* SKYMAP_PS = nullptr;
	ID3D11InputLayout* gInputLayout= nullptr;

	
	ID3D11DepthStencilState* DSLessEqual; //This is needed to make sure the sky is ALWAYS behind all other geometry
	ID3D11RasterizerState* RSCullNone; //Removes culling, (Might not be needed, culling is already deactivated)

	std::vector<Vertex> *vertices = nullptr;

	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	
	ID3D11Buffer* vertexBuffer = nullptr;

	XMFLOAT3 translation; // this needs to be the same as the camera. to make sure the viewer never reaches the sides
	XMFLOAT3 scale; //This will be a constant value
	XMFLOAT4X4 worldMatrix;
	worldConstantBuffer* worldStruct = nullptr;
	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine
	
	
	
	//Texture
	ID3D11ShaderResourceView* texture = nullptr;
	
	
	bool createCube();
	bool loadShaders();
	bool createRenderStates();

	bool loadTexture(std::string filePath);
public:
	SkyBox();
	bool Init(ID3D11DeviceContext* gDeviceContext, ID3D11Device *gDevice, ID3D11Buffer * worldBuffer, worldConstantBuffer* worldStruct);
	void Release();

	void update(XMFLOAT3 camPos);
	void render();

	virtual ~SkyBox();
};

