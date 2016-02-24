#pragma once
#include "Linker.h"
#include "Terrain.h"
//include frustrum here when it's done

class QuadTree
{
private:
	//Node struct
	struct NodeType
	{
		float posX, posY, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};

	//node Attributes
	int m_triangleCount, m_drawCount;
	Vertex *m_vertexList;
	NodeType* m_parentNode;

	//Functions
	void calculateMeshDimensions(int count, float &x, float &y, float &width);
	void createTreeNode(NodeType *parent, float x, float y, float width, ID3D11Device *gDevice);
	int countTriangles(float x, float y, float width);
	bool isTriangleContained(int count, float x, float y, float width);
	void ReleaseNode(NodeType *node);
	void RenderNode(NodeType *node, ID3D11DeviceContext *gDeviceContext, ID3D11Device *gDevice); //supposed to have frustrum aswell
public:
	QuadTree();
	QuadTree(const QuadTree &parent);
	~QuadTree();

	bool Initialize(Terrain *terrain, ID3D11Device *gDevice);
	void Release();
	void render(ID3D11DeviceContext *gDeviceContext, Terrain *terrain); //frustrumClass* supposed to be here too, also terrainShaderClass??

	int GetDrawCount();
};

