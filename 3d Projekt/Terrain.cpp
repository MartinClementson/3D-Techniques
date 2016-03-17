#include "Terrain.h"
#include <fstream>


void Terrain::copyIndexArray(void * indexList)
{
	memcpy(indexList, (void*)indices, sizeof(unsigned long)*m_vertexCount);
}

void Terrain::copyVertexArray(void * vertexList)
{
	memcpy(vertexList, (void*)vertices, sizeof(Vertex)*NumVertices);
	//return;
}

bool Terrain::initializeBuffers(ID3D11Device *gDevice)
{
	//declaring the buffer variables for later use
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//First determine the number of vertices and indices needed to construct the terrain with the given dimensions of the height map
	//Also we will construct triangles instead of quads out of the terrain so we will need six vertices to make two triangles per
	//quad with each triangle using triangle list.
	m_vertexCount = (heightMapWidth - 1) *(heightMapHeight - 1) * 6; 

	//allocating memory for the size of the indices to be used in the indexBuffer
	indices = new unsigned long[m_vertexCount];
	if (!indices)
		return false;

	//type casting the height and width to be used in for loops as well as calculations
	int cols = heightMapWidth;
	int rows = heightMapHeight;

	//Calculating the ammount of memory to allocate when stporing the Vertex points
	NumVertices = rows * cols;

	//Since rows and cols are the number of vertices for the width and length of our grid,
	//we need to subtract one from each then multiply them together to get the number of "quads" in our grid
	NumFaces = (rows - 1)*(cols - 1) * 2;

	vertices = new Vertex[NumVertices];

	for (UINT i = 0; i < rows; i++)
	{
		for (UINT j = 0; j < cols; j++)
		{
			vertices[i*cols + j].x = m_HeightMap[i*cols + j].x;
			vertices[i*cols + j].y = m_HeightMap[i*cols + j].y;
			vertices[i*cols + j].z = m_HeightMap[i*cols + j].z;
		}
	}


	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	//creating the uv with a techniques that makes them tile able and
	//also using the for loop to correctly supply the index buffer with the right order
	//of the Vertex Points
	for (UINT i = 0; i < (rows - 1); i++)
	{
		for (UINT j = 0; j < (cols - 1); j++)
		{
			indices[k] = (i+1)*cols + j;  // Top left of quad
			vertices[(i+1)*cols + j].u = (texUIndex + 0.0f);
			vertices[(i+1)*cols + j].v = (texVIndex + 0.0f);

			indices[k+1] = i*cols + j+1; // Bottom right of quad
			vertices[i*cols + j+1].u = (texUIndex + 1.0f);
			vertices[i*cols + j+1].v = (texVIndex + 1.0f);

			indices[k+2] = i*cols + j;  // Bottom left of quad
			vertices[i*cols + j].u = (texUIndex + 0.0f);
			vertices[i*cols + j].v = (texVIndex + 1.0f);




			indices[k + 3] = (i + 1)*cols + j; // Top left of quad
			vertices[(i + 1)*cols + j].u = (texUIndex + 0.0f);
			vertices[(i + 1)*cols + j].v = (texVIndex + 0.0f);

			indices[k + 4] = (i+1)*cols + j + 1; // Top right of quad
			vertices[(i+1)*cols + j + 1].u = (texUIndex + 1.0f);
			vertices[(i+1)*cols + j + 1].v = (texVIndex + 0.0f);
			indices[k + 5] = i*cols + j + 1;// Bottom right of quad
			vertices[i*cols + j + 1].u = (texUIndex + 1.0f);
			vertices[i*cols + j + 1].v = (texVIndex + 1.0f);


			k += 6;

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	////creating the vertex buffer to be based on the size of our terrain grid
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumVertices;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	////When using a vector as the buffers data, you have to supply a pointer to the first element in the vector
	//vertexData.pSysMem = vertices;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;

	//HRESULT hr;
	//hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	//if (FAILED(hr))
	//	return false;

	////Setup index buffer

	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(DWORD) * NumFaces * 3; 
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//indexData.pSysMem = &indices[0]; 
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//hr = gDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	//if (FAILED(hr))
	//	return false;

	//put in the DESTRUCTOR later
	//delete[] indices;
	//indices = 0;

	return true;
}

void Terrain::Release()
{

	//if (m_indexBuffer)
	//{
	//	m_indexBuffer->Release();
	//	m_indexBuffer = 0;

	//}
	//if (m_vertexBuffer)

	//{

	//	m_vertexBuffer->Release();
	//	m_vertexBuffer = 0;
	//}

	delete m_HeightMap;
}

void Terrain::Render(ID3D11DeviceContext * gDeviceContext)
{

	this->sendToConstantBuffer();
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	//setting the vertex and index buffers
	gDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	gDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	gDeviceContext->DrawIndexed(NumFaces * 3,0,0);

}

float Terrain::getYValue(float fX, float fZ)
{
	float returnY[4], upperY, lowerY;

	//getting the offset decimal value
	float deciX = fX - (float)(int)fX;
	float deciZ = fZ - (float)(int)fZ;

	//getting the y positions from the four points that define
	//the quad we are currently on.
	returnY[0] = m_HeightMap[((int)fZ*heightMapWidth) + (int)fX].y;
	returnY[1] = m_HeightMap[(((int)fZ+1)*heightMapWidth) + ((int)fX+1)].y;
	returnY[2] = m_HeightMap[(((int)fZ + 1)*heightMapWidth) + (int)fX].y;
	returnY[3] = m_HeightMap[((int)fZ*heightMapWidth) + ((int)fX + 1)].y;

	//interpolating the values on the X-axis
	upperY = (returnY[0] * (1.0 - deciX))+(returnY[3]*deciX);
	lowerY = (returnY[2] * (1.0 - deciX)) + (returnY[1] * deciX);

	//returning the interpolating value on the Z-axis
	return upperY*(1-deciZ)+lowerY*deciZ;
}

Terrain::Terrain()
{
	heightMapHeight = 512;
	heightMapWidth = 512;
	heightScale = 50; //control this later with antweakbar

	vertices = nullptr;
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
	It does this by averaging with 8 neighbour pixel, If we are at the edges, we take as many as possible
	using blurfilter
	
	| 1 | 2 | 3 |
	-------------
	| 4 | ij| 6 |
	-------------
	| 7 | 8 | 9 |
	*/
	
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
	
	//apply average to each heighmap entry
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



void Terrain::updateWorldMatrix()
{


	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//We convert from degrees to radians here. Before this point we work in degrees to make it easier for the programmer and user
	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(toRadian(rotation.x));
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(toRadian(rotation.y));
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(toRadian(rotation.z));


	



	//Create the rotation matrix
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
	rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);


	//Intoduce the world matrix, multiply rotation and scale. (world translation comes later)
	DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(rotationMatrix, scaleMatrix);


	//Create the world translation matrix
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

	//Multiply the (scale*rotation) matrix with the world translation matrix
	world = DirectX::XMMatrixMultiply(world, translationMatrix);

	XMVECTOR worldDet = XMMatrixDeterminant(world);
	XMStoreFloat4x4(&normalWorld, XMMatrixInverse(&worldDet, world));


	world = XMMatrixTranspose(world);


	XMStoreFloat4x4(&this->worldMatrix, world);
}

void Terrain::sendToConstantBuffer()
{
	this->worldStruct->world = this->worldMatrix;
	this->worldStruct->normalWorld = this->normalWorld;
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	worldConstantBuffer* temporaryWorld = (worldConstantBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	this->gDeviceContext->Unmap(worldBuffer, 0);



}

bool Terrain::init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext, ID3D11Buffer* worldBuffer)
{
	bool result;

	this->worldBuffer = worldBuffer;
	this->gDeviceContext = gDeviceContext;
	this->worldStruct = new worldConstantBuffer;
	//creating a vector to hold the whole .raw file
	std::vector<unsigned char> in(heightMapHeight * heightMapWidth);

	//opening the heightMap file and setting the ifstream variable to read it in binary form
	std::ifstream loading;
	loading.open("terrain.raw", std::ios_base::binary);
	if (!loading)
		return false;
	else
	{
		//reading the whole file by starting from 0 and continuing until the size of the vector (which is as big as the file) 
		loading.read((char*)&in[0], (std::streamsize)in.size());

		loading.close();
	}

	//Allocating the set memory for a float vector to hold the newly aquired infromation
	mHeightMap.resize((heightMapHeight * heightMapWidth), 0);
	for (UINT i = 0; i < (heightMapHeight * heightMapWidth); ++i)
	{
		//parsing the value of each pixel into the vector and multiplying it with our scale, and also normalizing it
		mHeightMap.at(i) = (in.at(i) / 255.0f)*heightScale;
	}

	//allocating new memory to a struct pointer that will hold the information for creating the grid
	m_HeightMap = new HeightMapType[heightMapHeight * heightMapWidth];
	if (!m_HeightMap)
		return false;

	//smoothing the loaded "terrain" for a "less pointy" result
	smooth();


	//Putting the Heightmap infromation into the vector to be used later when creating the grid
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

	//emptying the variable, that holds the float informtation from the height map
	mHeightMap.clear();

	//initializing the buffers and creating the grid
	result = initializeBuffers(gDevice);
	if (!result)
		return false;
	this->updateWorldMatrix();
	return true;
}

Terrain::~Terrain()
{
	delete[] indices;
	delete[] vertices;
	delete this->worldStruct;
}
