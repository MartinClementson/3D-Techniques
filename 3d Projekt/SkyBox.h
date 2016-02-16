#pragma once
#include "Linker.h"


#pragma region Comment
//pragma region makes the text collapsable
/*
	The skybox is pretty straight forward, here we intoduce cube textures.

	a cube texture. (a.k.a, cube mapping) is a texture file that is built up
	of 6 individual pictures. 
	https://upload.wikimedia.org/wikipedia/commons/b/b4/Skybox_example.png


	To find which texel goes to which pixel, UV coordinates are not used as they are
	normally. A 3d vector will be used.

	The easy solution to that in this project, is that we take the vertex local position
	as a direction vector that points to the texel to be used.


	Furthermore, We create a cube and scale it up.
	This cube needs to have its own depthstencilState and rasterizerstate, to make
	it behave as a skybox should, for example always be behind all other geometry, etc.

	The cube needs to be centered around the camera all the time. therefore the cameras position
	is connected to the cube. 

	It is also important that the box is as far away as it can be. Therefore, in the geometryshader
	we give the z value the value of w. so that z always gets the value 1. which
	is the farthest possible depth.


*/

#pragma endregion Description of class

using namespace DirectX;
class SkyBox
{

private:


	//Pointer to the device and context
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext =nullptr;


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

