#include "Terrain.h"
#include <fstream>


bool Terrain::initializeBuffers(ID3D11Device *gDevice)
{
	int i, j; //maybe not needed
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int index1, index2, index3, index4;

	m_vertexCount = (heightMapWidth - 1) *(heightMapHeight - 1) * 12; //because it's going to be a lineList
	m_indexCount = m_vertexCount;

	vertices = new Vertex[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	index = 0;

	for (j = 0; j < (heightMapHeight - 1); j++)
	{
		for (i = 0; i < (heightMapWidth - 1); i++)
		{
			index1 = (heightMapHeight * j) + i;				//bottom left
			index2 = (heightMapHeight * j) + (i + 1);		//bottom right
			index3 = (heightMapHeight * (j + 1)) + i;		//Upper Left
			index4 = (heightMapHeight * (j + 1)) + (i + 1); //Upper Right

			//Upper Left
			vertices[index].x = m_HeightMap[index3].x;
			vertices[index].y = m_HeightMap[index3].y;
			vertices[index].z = m_HeightMap[index3].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].x = m_HeightMap[index4].x;
			vertices[index].y = m_HeightMap[index4].y;
			vertices[index].z = m_HeightMap[index4].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].x = m_HeightMap[index4].x;
			vertices[index].y = m_HeightMap[index4].y;
			vertices[index].z = m_HeightMap[index4].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom left
			vertices[index].x = m_HeightMap[index1].x;
			vertices[index].y = m_HeightMap[index1].y;
			vertices[index].z = m_HeightMap[index1].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom left
			vertices[index].x = m_HeightMap[index1].x;
			vertices[index].y = m_HeightMap[index1].y;
			vertices[index].z = m_HeightMap[index1].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Upper Left
			vertices[index].x = m_HeightMap[index3].x;
			vertices[index].y = m_HeightMap[index3].y;
			vertices[index].z = m_HeightMap[index3].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom Left
			vertices[index].x = m_HeightMap[index1].x;
			vertices[index].y = m_HeightMap[index1].y;
			vertices[index].z = m_HeightMap[index1].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].x = m_HeightMap[index4].x;
			vertices[index].y = m_HeightMap[index4].y;
			vertices[index].z = m_HeightMap[index4].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].x = m_HeightMap[index4].x;
			vertices[index].y = m_HeightMap[index4].y;
			vertices[index].z = m_HeightMap[index4].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom Right
			vertices[index].x = m_HeightMap[index2].x;
			vertices[index].y = m_HeightMap[index2].y;
			vertices[index].z = m_HeightMap[index2].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom Right
			vertices[index].x = m_HeightMap[index2].x;
			vertices[index].y = m_HeightMap[index2].y;
			vertices[index].z = m_HeightMap[index2].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;

			//Bottom Left
			vertices[index].x = m_HeightMap[index1].x;
			vertices[index].y = m_HeightMap[index1].y;
			vertices[index].z = m_HeightMap[index1].z;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			indices[index] = index;
			index++;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hr))
		return false;

	//Setup index buffer

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = gDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	if (FAILED(hr))
		return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Terrain::Release()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;

	}
	if (m_vertexBuffer)

	{

		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	//heightMapRSV->Release();
	delete m_HeightMap;
}

void Terrain::Render(ID3D11DeviceContext * gDeviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	gDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	gDeviceContext->DrawIndexed(m_indexCount,0,0);

}

Terrain::Terrain()
{
	heightMapHeight = 1024;
	heightMapWidth = 1024;
	heightScale = 50;

	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_HeightMap = nullptr;
}

bool Terrain::inBounds(int i, int j)
{
	// true if ij are valid indices, false ortherwise

	return
		i >= 0 && i < (int)heightMapHeight &&
		j >= 0 && j < (int)heightMapWidth;
	
}

float Terrain::average(int i, int j)
{
	/*This function computes the average height oj the ij element.
	It does this by averaging with 8 neighbour pixel, If we are at the edges, we take as many as possible*/
	
	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg += mHeightMap[m * heightMapWidth + n];
				num += 1;

			}

		}

	}
	
	
	return avg/num;
}

void Terrain::smooth()
{
	int i, j;

	for (j = 0; j < heightMapHeight; j++)
	{
		for (i = 0; i < heightMapWidth; i++)
		{
			m_HeightMap[(heightMapHeight*j) + i].y /= 15.0f;
		}
	}
	//std::vector<float> dest(mHeightMap.size());
	//for (UINT i = 0; i < heightMapHeight; i++)
	//{

	//	for (UINT j = 0; j < heightMapWidth; j++)
	//	{

	//		dest[i * heightMapWidth + j] = average(i, j);

	//	}

	//}
	////replace the old heightmap with the new filtered one
	//mHeightMap = dest;

}

void Terrain::createPoints()
{
}

bool Terrain::init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext)
{
	bool result;
	std::vector<unsigned char> in(heightMapHeight * heightMapWidth);
	std::ifstream loading;
	loading.open("terrain.raw", std::ios_base::binary);
	if (!loading)
		return false;
	else
	{
		loading.read((char*)&in[0], (std::streamsize)in.size());

		loading.close();
	}
	mHeightMap.resize((heightMapHeight * heightMapWidth), 0);
	for (UINT i = 0; i < (heightMapHeight * heightMapWidth); ++i)
	{

		mHeightMap.at(i) = (in.at(i) / 255.0f)*heightScale;
	}

	m_HeightMap = new HeightMapType[heightMapHeight * heightMapWidth];
	if (!m_HeightMap)
		return false;

	for (int j = 0; j < heightMapHeight; j++)
	{
		for (int i = 0; i < heightMapWidth; i++)
		{
			index = (heightMapHeight * j) + i;

			m_HeightMap[index].x = (float)i;
			m_HeightMap[index].y = mHeightMap[j + i];
			m_HeightMap[index].z = (float)j;

		}
	}

	//delete[] mHeightMap;
	mHeightMap.clear();

	smooth(); //kolla in denna, kommer inte att fungera

	result = initializeBuffers(gDevice);
	if (!result)
		return false;

	return true;
}

Terrain::~Terrain()
{
}
