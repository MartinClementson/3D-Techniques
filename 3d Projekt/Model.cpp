#include "Model.h"




Model::Model()
{
	
}

void Model::createVertices(ID3D11Device* gDevice)
{
}
Model::Model(ID3D11DeviceContext * gDeviceContext, ID3D11Buffer * worldBuffer, worldConstantBuffer* worldStruct)
{

	this->vertices = new std::vector<Vertex>;
	this->gDeviceContext = gDeviceContext;
	XMStoreFloat4x4(&this->worldMatrix, XMMatrixIdentity());
	this->worldBuffer = worldBuffer;
	this->worldStruct = worldStruct;
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
Model::Model(const Model &obj) //Copy Constructor
{
	this->pivotPoint = obj.pivotPoint;
	this->translation = obj.translation;
	this->rotation = obj.rotation;
	this->scale = obj.scale;
	this->worldMatrix = obj.worldMatrix;
	this->vertices = new std::vector<Vertex>;

	for (size_t i = 0; i < obj.vertices->size(); i++)
	{
		this->vertices->push_back(obj.vertices->at(i));
	}


}


void Model::setPivotPoint(XMFLOAT3 newPosition)
{

	this->pivotPoint = newPosition;

}

void Model::setTranslation(XMFLOAT3 newTranslation)
{
	this->translation = newTranslation;
	updateWorldMatrix();
}

void Model::setRotation(XMFLOAT3 degrees)
{
	this->rotation = degrees;
	updateWorldMatrix();
}

void Model::setScale(XMFLOAT3 newScale) //Fix and make it non uniform
{
	this->scale = newScale;
	updateWorldMatrix();
}

XMFLOAT3 Model::getPivotPoint()
{
	return this->pivotPoint;

}

XMFLOAT3 Model::getTranslation()
{
	return this->translation;
}

XMFLOAT3 Model::getRotation()
{
	return this->translation;
}




Model::~Model()
{

	delete vertices;
	this->vertexBuffer->Release();
}

void Model::update()
{
	this->worldStruct->world = this->worldMatrix;
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	worldConstantBuffer* temporaryWorld = (worldConstantBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	this->gDeviceContext->Unmap(worldBuffer, 0);

	this->gDeviceContext->VSSetConstantBuffers(0, 1, &worldBuffer); //change to geometry shader later




	//


}

void Model::render()
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->Draw(this->vertices->size(), 0); //This will be dynamic,


}

void Model::updateWorldMatrix()
{

	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	
	//We convert from degrees to radians here. Before this point we work in degrees to make it easier for the programmer and user
	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(toRadian(rotation.x));
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(toRadian(rotation.y));
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(toRadian(rotation.z));

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
	rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);


	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);


	DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(rotationMatrix, scaleMatrix);
	world = DirectX::XMMatrixMultiply(world, translationMatrix);
	world =XMMatrixTranspose(world);

	XMStoreFloat4x4(&this->worldMatrix, world);
}


