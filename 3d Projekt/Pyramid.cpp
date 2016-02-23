#include "Pyramid.h"



void Pyramid::createVertices(ID3D11Device* gDevice)
{
	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		0.0f, 0.5f , 0.0f, //0
		1.0f, 1.0f, 1.0f
	});

	this->vertices->push_back(Vertex
	{
		
		-0.5f, -0.5f , 0.5f, //1
		0.0f, 0.0f,   1.0f
	});
	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , 0.5f, //2
		0.0f, 1.0f,   0.0f
	});

	this->vertices->push_back(Vertex
	{
		
		0.5f, -0.5f , -0.5f,//3
		1.0f, 0.0f,   0.0f
	});



	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f, //4
		0.0f, 1.0f,   1.0f
	});


	

	UINT in[18] //Index buffer
	{
		0,1,2, // first tris
		0,2,3, // second tris
		0,3,4, // third tris
		0,4,1, // fourth tris
		1,4,3, // fifth tris
		2,1,3 // sixth tris
	};

	


	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* vertices->size();


	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = vertices->data();
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."

	gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);





	//create indexbuffer

	D3D11_BUFFER_DESC ibd;

	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 18;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indData;
	indData.pSysMem = in;

	gDevice->CreateBuffer(&ibd, &indData, &indexBuffer);


}

Pyramid::Pyramid()
{
	this->vertices = new std::vector<Vertex>;
	//this->createVertices();
}

Pyramid::Pyramid(ID3D11Device* gDevice,ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct)
	: Model(gDeviceContext, worldBuffer, worldStruct)
{

	this->createVertices(gDevice);

}

Pyramid::Pyramid(const Pyramid & obj)
{

	this->pivotPoint = obj.pivotPoint;
	this->translation = obj.translation;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->vertices = new std::vector<Vertex>;
	for (size_t i = 0; i < obj.vertices->size(); i++)
	{
		this->vertices->push_back(obj.vertices->at(i));
	}

}


Pyramid::~Pyramid()
{
	this->indexBuffer->Release();

}

void Pyramid::update()
{
	
	
	
	
	return Model::update();
	
}

void Pyramid::render()

{


	this->sendToConstantBuffer();
	this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer);

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->gDeviceContext->DrawIndexed(18, 0, 0); //should be 5?


	//return Model::render();

}

