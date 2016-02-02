#include "Model.h"
#include "bth_image.h"




Model::Model()
{
	
}
void Model::loadTexture(ID3D11Device* gDevice)
{
	
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));

	textDesc.Width = BTH_IMAGE_WIDTH;
	textDesc.Height = BTH_IMAGE_HEIGHT;
	textDesc.MipLevels = textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textDesc.MiscFlags = 0;
	textDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	data.pSysMem = (void*)BTH_IMAGE_DATA;
	data.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);
	HRESULT hr = gDevice->CreateTexture2D(&textDesc, &data, &pTexture);


	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;

	ZeroMemory(&resViewDesc, sizeof(resViewDesc));
	resViewDesc.Format = textDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = textDesc.MipLevels;
	resViewDesc.Texture2D.MostDetailedMip = 0;
	hr = gDevice->CreateShaderResourceView(pTexture, &resViewDesc, &texture);

	pTexture->Release();


}

//This is the constructor for OBJ import
Model::Model(std::string filePath, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext, ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct)
{

	this->vertices = new std::vector<Vertex>;
	this->gDeviceContext = gDeviceContext;
	XMStoreFloat4x4(&this->worldMatrix, XMMatrixIdentity());
	this->worldBuffer = worldBuffer;
	this->worldStruct = worldStruct;
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	ObjHandler* importer = new ObjHandler(filePath, *this);

	//Make import

	delete importer; // delete when done;
	loadTexture(gDevice);

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

	//Create texture

	HRESULT result;
	
}

//This is the constructor for Primitives
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

void Model::setVertex(Vertex &nVertex)
{
	this->vertices->push_back(nVertex);
}

void Model::createVertices(ID3D11Device* gDevice)
{
}

void Model::setRotateState(bool state)
{
	this->rotate = state;
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

	if (rotate)
	{
		float static angle = 0; //<----- just temporary to test rotation
		angle += 0.01f;
		this->setRotation(XMFLOAT3(0, angle, 0));


	}
	this->worldStruct->world = this->worldMatrix;
	this->worldStruct->normalWorld = this->normalWorld;

	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	worldConstantBuffer* temporaryWorld = (worldConstantBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	this->gDeviceContext->Unmap(worldBuffer, 0);

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer); 

}

void Model::render()
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//IF there is a texture. apply it to the pixel shader

	if (texture != nullptr) {

		this->gDeviceContext->PSSetShaderResources(0, 1, &this->texture);
	}


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
	XMVECTOR worldDet = XMMatrixDeterminant(world);
	XMStoreFloat4x4(&normalWorld, XMMatrixInverse(&worldDet, world));
	
	
	world =XMMatrixTranspose(world);


	XMStoreFloat4x4(&this->worldMatrix, world);
}


