#include "Terrain.h"
#include <fstream>


bool Terrain::initializeBuffers(ID3D11Device *gDevice)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	m_vertexCount = (heightMapWidth - 1) *(heightMapHeight - 1) * 6; 


	indices = new unsigned long[m_vertexCount];
	if (!indices)
		return false;

	int cols = heightMapWidth;
	int rows = heightMapHeight;

	NumVertices = rows * cols;
	NumFaces = (rows - 1)*(cols - 1) * 2;

	std::vector<Vertex> v(NumVertices);

	for (DWORD i = 0; i < rows; i++)
	{
		for (DWORD j = 0; j < cols; j++)
		{
			v[i*cols + j].x = m_HeightMap[i*cols + j].x;
			v[i*cols + j].y = m_HeightMap[i*cols + j].y;
			v[i*cols + j].z = m_HeightMap[i*cols + j].z;
		}
	}


	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	for (DWORD i = 0; i < (rows - 1); i++)
	{
		for (DWORD j = 0; j < (cols - 1); j++)
		{
			indices[k] = (i+1)*cols + j;  // Top left of quad
			v[(i+1)*cols + j].u = (texUIndex + 0.0f);
			v[(i+1)*cols + j].v = (texVIndex + 0.0f);

			indices[k+1] = i*cols + j+1; // Bottom right of quad
			v[i*cols + j+1].u = (texUIndex + 1.0f);
			v[i*cols + j+1].v = (texVIndex + 1.0f);

			indices[k+2] = i*cols + j;  // Bottom left of quad
			v[i*cols + j].u = (texUIndex + 0.0f);
			v[i*cols + j].v = (texVIndex + 1.0f);




			indices[k + 3] = (i + 1)*cols + j; // Top left of quad
			v[(i + 1)*cols + j].u = (texUIndex + 0.0f);
			v[(i + 1)*cols + j].v = (texVIndex + 0.0f);

			indices[k + 4] = (i+1)*cols + j + 1; // Top right of quad
			v[(i+1)*cols + j + 1].u = (texUIndex + 1.0f);
			v[(i+1)*cols + j + 1].v = (texVIndex + 0.0f);
			indices[k + 5] = i*cols + j + 1;// Bottom right of quad
			v[i*cols + j + 1].u = (texUIndex + 1.0f);
			v[i*cols + j + 1].v = (texVIndex + 1.0f);


			k += 6;

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &v[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hr))
		return false;

	//Setup index buffer

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumFaces * 3; 
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &indices[0]; 
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = gDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	if (FAILED(hr))
		return false;


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

	gDeviceContext->DrawIndexed(NumFaces * 3,0,0);

}

Terrain::Terrain()
{
	heightMapHeight = 512;
	heightMapWidth = 512;
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
	std::vector<float> dest(mHeightMap.size());
	for (UINT i = 0; i < heightMapHeight; i++)
	{

		for (UINT j = 0; j < heightMapWidth; j++)
		{

			dest[i * heightMapWidth + j] = average(i, j);

		}

	}
	//replace the old heightmap with the new filtered one
	mHeightMap = dest;

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

	smooth();

	for (int j = 0; j < heightMapHeight; j++)
	{
		for (int i = 0; i < heightMapWidth; i++)
		{
			index = (heightMapHeight * j) + i;

			m_HeightMap[index].x = (float)i;
			m_HeightMap[index].y = mHeightMap[index];
			m_HeightMap[index].z = (float)j;

		}
	}

	
	mHeightMap.clear();


	result = initializeBuffers(gDevice);
	if (!result)
		return false;

	return true;
}

Terrain::~Terrain()
{
}
