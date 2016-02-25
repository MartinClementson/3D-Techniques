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

	//Functions
	void calculateMeshDimensions(int count, float &x, float &z, float &meshWidth);
	void createTreeNode(NodeType *parent, float x, float z, float width, ID3D11Device *gDevice);
	int countTriangles(float x, float y, float width);
	bool isTriangleContained(int count, float x, float y, float width);
	void ReleaseNode(NodeType *node);
	void RenderNode(NodeType *node, ID3D11DeviceContext *gDeviceContext, Terrain *terrain, Frustum* frustum);
public:
	QuadTree();
	QuadTree(const QuadTree &parent);
	~QuadTree();

	bool Initialize(Terrain *terrain, ID3D11Device *gDevice);
	void Release();
	void render(ID3D11DeviceContext *gDeviceContext, Terrain *terrain, Frustum * frustum); 

	int GetDrawCount();
};

