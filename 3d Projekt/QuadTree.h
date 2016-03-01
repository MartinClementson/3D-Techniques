#pragma once
#include "Linker.h"
#include "Terrain.h"
#include "Frustum.h"


//GLOBAL
const int maxTriangles = 10000; //The maximum amount of triangles in a node
//when no node has more than this amount, We stop subdividing the quad tree
// Less Max triangles == more nodes == more memory used

class QuadTree
{
private:
	
	ID3D11DeviceContext* gDeviceContext;


	XMFLOAT3 translation = { -285.0f, -6.0f, -210 };
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	worldConstantBuffer* worldStruct;


	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 normalWorld;


	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine

	void updateWorldMatrix();

	void sendToConstantBuffer();


	//Node struct
	struct NodeType
	{
		float posX, posZ, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};

	//node Attributes
	int m_triangleCount, m_drawCount;
	Vertex *m_vertexList; // This is the vertices from the Terrain
	NodeType* m_parentNode;
	unsigned long *m_indexList;
	int indexCount;
	//Functions
	void calculateMeshDimensions(int count, float &x, float &z, float &meshWidth);
	void createTreeNode(NodeType *parent, float x, float z, float width, ID3D11Device *gDevice);
	int countTriangles(float x, float z, float width);
	bool isTriangleContained(int index, float x, float z, float width);
	void ReleaseNode(NodeType *node);
	void RenderNode(NodeType *node, ID3D11DeviceContext *gDeviceContext, Frustum* frustum, ID3D11Buffer* worldBuffer);
public:
	QuadTree();
	QuadTree(const QuadTree &parent);
	~QuadTree();

	bool Initialize(Terrain *terrain, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext, ID3D11Buffer* worldBuffer);
	void Release();
	void render(ID3D11DeviceContext *gDeviceContext, Frustum * frustum, ID3D11Buffer* worldBuffer);

	int GetDrawCount();
};

