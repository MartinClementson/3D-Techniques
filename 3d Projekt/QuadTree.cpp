#include "QuadTree.h"



void QuadTree::calculateMeshDimensions(int count, float & x, float & z, float & meshWidth)
{
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

	//Center position of the mesh, Start at zero
	x = 0.0f;
	z = 0.0f;

	//Sum all the vertices in the mesh
	for (int i = 0; i < count; i++)
	{
		x += m_vertexList[i].x;
		z += m_vertexList[i].z;
	}
	//Divide the sum with the number of vertices to find the mid point in the mesh
	x = x / (float)count;
	z = z / (float)count;

	//Init the max and min size of the mesh
	maxWidth = 0.0f;
	maxDepth = 0.0f;

	//fabsf Return the absolute value of the argument as FLOAT
	minWidth = fabsf(m_vertexList[0].x - x);
	minDepth = fabsf(m_vertexList[0].z - z);

	//Loop through all the vertices and find the max/min width and depth
	for (int i = 0; i < count; i++)
	{
		
		width = fabsf(m_vertexList[i].x - x);
		depth = fabsf(m_vertexList[i].z - z);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}

	//Find the absolute max calue between the min and max depth and width
	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));
	

	//Calculate the maximum diameter of the mesh
	meshWidth = max(maxX, maxZ) * 2.0f;

	return;
}



void QuadTree::createTreeNode(NodeType * parent, float x, float z, float width, ID3D11Device * gDevice)
{
	/* This function Builds the quad tree. It is recursive and therefore it will call itself numerous times
	   It starts with the parent, Then goes down.
	   When it reaches a leaf node, it loads the vertex data into that node
	*/

	int numTriangles, i, count, vertexCount, index, vertexIndex;

	float offsetX, offsetZ;

	Vertex* vertices;

	unsigned long* indices;

	bool result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Initialize the node and set it's position in the world

	//Store the node position and size
	parent->posX = x;
	parent->posZ = z;
	parent->width = width;

	//Set triangle count to Zero

	parent->triangleCount = 0;

	//Set index and vertex buffers to null;
	parent->indexBuffer = 0;
	parent->vertexBuffer = 0;

	//Set the children nodes of this node to null
	parent->nodes[0] = 0;
	parent->nodes[1] = 0;
	parent->nodes[2] = 0;
	parent->nodes[3] = 0;

	//Count the number of triangles that are inside this node
	numTriangles = countTriangles(x, z, width);

	//Case 1: If there are no triangles in this node, return it as empty
	if (numTriangles = 0)
		return;
	
	//Case 2: IF there are too many triangles in this node, then split it into four smaller nodes
	if (numTriangles > maxTriangles)
	{
		for (i = 0; i < 4; i++)
		{
			//Calculate position offset for the new child node
			if ((i % 2) < 1)							//
				offsetX = -1.0f * (width / 4.0f);		// c-style representation would be
			else                                        // offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
				offsetX = 1.0f * (width / 4.0f);        // condition ? valueIfTrue : valueIfFalse
														// Changed to c++ if statement for easier understanding

			if ((i % 4) < 2)
				offsetZ = -1.0f * (width / 4.0f);
			else
				offsetZ =  1.0f * (width / 4.0f);

			//See if there are any triangles in the new node
			count = countTriangles((x + offsetX), (z + offsetZ), (width / 2));

			if (count > 0)
			{
				//If there are triangles inside where this new node would be, then we create the child node
				parent->nodes[i] = new NodeType;

				//Extend the tree starting from this new child node
				createTreeNode(parent->nodes[i], (x + offsetX), (z + offsetZ), (width / 2),gDevice);
			}
			
		}
		return;
	}

	//Case 3: If there are the right number of triangles, then create and load the vertex and index buffer
	//from the terrain list into this node.. (We have determined that this is a leaf node)

	parent->triangleCount = numTriangles;

	//Calculate the number of vertices
	vertexCount = numTriangles * 3;

	//Create vertex array
	vertices = new Vertex[vertexCount];

	//Create the index array
	indices = new unsigned long[vertexCount];

	//Initialize the index
	index = 0;

	//Loop through all the triangles in the vertex list

	for (i = 0; i < m_triangleCount; i++)
	{
		//If the triangle is inside this node then add it to the vertex array
		result = isTriangleContained(i, x, z, width);

		if (result == true)
		{
			//Calculate the index into the terrain vertex list
			vertexIndex = i * 3;

			//Get the three vertices of this triangle from the vertex list.
			vertices[index] = m_vertexList[vertexIndex]; //a operator= overload was made to minimize the code. (check struct definition)
			indices[index] = index;
			index++;
			
			vertexIndex++;
			vertices[index] = m_vertexList[vertexIndex];
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index] = m_vertexList[vertexIndex];
			indices[index] = index;
			index++;


		}


	}

	//Set up the description for the vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	//Create the vertex buffer
	gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &parent->vertexBuffer);

	//Set up the description of the index buffer

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create index buffer
	gDevice->CreateBuffer(&indexBufferDesc, &indexData, &parent->indexBuffer);

	//Delete the vertices and indices arrays, as they are now stored in the buffers

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return;



}

int QuadTree::countTriangles(float x, float z, float width)
{
	return 0;
}

bool QuadTree::isTriangleContained(int count, float x, float z, float width)
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
