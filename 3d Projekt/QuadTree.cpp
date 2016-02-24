#include "QuadTree.h"



void QuadTree::calculateMeshDimensions(int count, float & x, float & y, float & width)
{
}

void QuadTree::createTreeNode(NodeType * parent, float x, float y, float width, ID3D11Device * gDevice)
{
}

int QuadTree::countTriangles(float x, float y, float width)
{
	return 0;
}

bool QuadTree::isTriangleContained(int count, float x, float y, float width)
{
	return false;
}

void QuadTree::ReleaseNode(NodeType * node)
{
}

void QuadTree::RenderNode(NodeType * node, ID3D11DeviceContext * gDeviceContext, ID3D11Device * gDevice)
{
}

QuadTree::QuadTree()
{
	m_vertexList = nullptr;
	m_parentNode = nullptr;
}

QuadTree::QuadTree(const QuadTree &parent)
{
}


QuadTree::~QuadTree()
{
}

bool QuadTree::Initialize(Terrain * terrain, ID3D11Device * gDevice)
{
	int vertexCount;
	float centerX, centerY, width;
	return false;

	vertexCount = terrain->getVertexCount();
	if (!m_vertexList)
		return false;

	terrain->copyVertexArray((void*)m_vertexList);
}

void QuadTree::Release()
{
}

void QuadTree::render(ID3D11DeviceContext * gDeviceContext, Terrain * terrain)
{
}

int QuadTree::GetDrawCount()
{
	return 0;
}
