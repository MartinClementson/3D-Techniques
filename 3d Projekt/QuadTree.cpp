#include "QuadTree.h"



void QuadTree::calculateMeshDimensions(int count, float & x, float & z, float & meshWidth)
{
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

	x = 0.0f;
	z = 0.0f;
	for (int i = 0; i < count; i++)
	{
		x += m_vertexList[i].x;
		z += m_vertexList[i].z;
	}
	x = x / (float)count;
	z = z / (float)count;

	maxWidth = 0.0f;
	maxDepth = 0.0f;

	minWidth = fabsf(m_vertexList[0].x - x);
	minDepth = fabsf(m_vertexList[0].z - z);

	for (int i = 0; i < count; i++)
	{
		width = fabsf(m_vertexList[i].x - x);
		depth = fabsf(m_vertexList[i].z - z);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}

	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	meshWidth = max(maxX, maxZ) * 2.0f;

	return;
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

void QuadTree::RenderNode(NodeType * node, ID3D11DeviceContext * gDeviceContext, Terrain *terrain, Frustum* frustum)
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
	delete[] m_vertexList;
}

bool QuadTree::Initialize(Terrain * terrain, ID3D11Device * gDevice)
{
	int vertexCount;
	float centerX, centerY, width;
	

	//Get the number of vertices in the terrain
	vertexCount = terrain->getVertexCount();
	
	//Store the total triangle count
	m_triangleCount = vertexCount / 3;
	
	//create a vertex array to hold all of the terrain vertices
	m_vertexList = new Vertex[vertexCount];
	if (!m_vertexList)
		return false;

	//Copy the vertices from the terrain into the vertex list
	terrain->copyVertexArray((void*)m_vertexList);

	//Calculate the parent node. It's the upper most quad, covering the whole terrain
	//Calculates center x,z and width
	calculateMeshDimensions(vertexCount, centerX, centerY, width);

	//Create the parent node of the mesh
	m_parentNode = new NodeType;
	if (!m_parentNode)
		return false;

	//Recursively build the quad tree, based on the vertex list and mesh dimensions
	createTreeNode(m_parentNode, centerX, centerY, width, gDevice);

	//Now the vertex list is no longer needed
	if (m_vertexList)
	{
		delete []m_vertexList;
		m_vertexList = 0;
	}

	return true;
}

void QuadTree::Release()
{
	//Recursively release the quad tree data
	if (m_parentNode)
	{
		ReleaseNode(m_parentNode);
		delete m_parentNode;
		m_parentNode = 0;
	}
	return;
}

void QuadTree::render(ID3D11DeviceContext * gDeviceContext, Terrain * terrain, Frustum* frustum)
{
	//reset the number of triangles drawn for this frame
	m_drawCount = 0;

	//Render each node that is visible, starting at the parent node and moving down the tree
	RenderNode(m_parentNode, gDeviceContext, terrain, frustum); 
}

int QuadTree::GetDrawCount() //THis returns the number of triangles that were drawn in the previous render function call
{
	return m_drawCount;
}
