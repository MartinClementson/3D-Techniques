#pragma once
#include "Linker.h"

using namespace DirectX;
// http://www.rastertek.com/tertut02.html
class Terrain
{
private:


	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;


	XMFLOAT3 translation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	worldConstantBuffer* worldStruct;


	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 normalWorld;


	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine


	float heightScale;
	float cellSpacing;
	UINT heightMapWidth;
	UINT heightMapHeight;

	//from the tutorial
	struct HeightMapType
	{
		float x, y, z;
	};
	UINT m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_HeightMap;
	int index;
	Vertex* vertices;
	unsigned long* indices;

	//a different tutorial
	int NumVertices;
	int NumFaces;

	std::vector<float> mHeightMap;
	bool inBounds(int i, int j);
	float average(int i, int j);
	void smooth();
	

	void updateWorldMatrix();
	void sendToConstantBuffer();

public:

	int getIndexCount() { return m_vertexCount; }
	void copyIndexArray(void* indexList);
	void copyVertexArray(void* vertexList);
	int getVertexCount() { return NumFaces; }
	bool initializeBuffers(ID3D11Device *gDevice);
	void Release();
	void Render(ID3D11DeviceContext *gDeviceContext);
	Terrain();
	bool init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext, ID3D11Buffer* worldBuffer);
	~Terrain();
	//int getIndexCount() { return m_indexCount; }

	void setTranslation(XMFLOAT3 translate) { this->translation = translate; updateWorldMatrix(); };
};

