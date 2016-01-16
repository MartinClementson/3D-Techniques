#include "Plane.h"



Plane::Plane()
{

	


}

Plane::Plane(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11Buffer* worldBuffer, worldConstantBuffer* worldStruct)
	: Model(gDeviceContext, worldBuffer, worldStruct)
{
	
	
	this->createVertices(gDevice);
	
}

void Plane::createVertices(ID3D11Device* gDevice)
{
	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.0f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.0f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , 0.0f, PAD,
		0.0f, 0.0f,   1.0f, PAD
	});


	//Second Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.0f, PAD,
		1.0f, 0.0f, 0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, 0.5f , 0.0f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.0f, PAD,
		0.0f, 1.0f,   0.0f, PAD
	});

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


}

Plane::~Plane()
{
	
	
}



void Plane::update()
{

	float static angleX = 0; //<----- just temporary to test rotation
	angleX += 0.0001;
	DirectX::XMStoreFloat4x4(&this->worldMatrix, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(angleX)));


	return Model::update();
}

void Plane::render()
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->Draw(this->vertices->size(), 0); //This will be dynamic,

}

Plane::Plane(const Plane & obj) //Copy constructor
{
	this->pivotPoint = obj.pivotPoint;
	this->translation = obj.translation;
	this->rotation = obj.rotation;
	this->scale = obj.scale;
	this->gDeviceContext = obj.gDeviceContext;
	
	//More might need to be added later!

	this->vertices = new std::vector<Vertex>;
	for (size_t i = 0; i < obj.vertices->size(); i++)
	{
		this->vertices->push_back(obj.vertices->at(i));
	}


}

