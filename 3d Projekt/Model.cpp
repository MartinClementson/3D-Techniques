#include "Model.h"
#include "bth_image.h"
#include "DirectXToolkit.h"

#include "objHandler.h"





Model::Model()
{
	
}
//this is the constructor for the children in the obj importer
Model::Model(std::vector<Vertex> *vertArray, std::string * texturePath, ID3D11Device* gDevice,
	ID3D11DeviceContext * gDeviceContext, ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, std::vector<UINT> &indices)
{
	
	this->vertices = new std::vector<Vertex>;

	for (int i = 0; i < vertArray->size(); i++)
	{
		//Not sure if this makes a copy. it should be calling the copy constructor
		//if it doesent copy, the vertices will be removed when the obj import is done
		setVertex(vertArray->at(i));
		
	}

	this->gDeviceContext = gDeviceContext;
	XMStoreFloat4x4(&this->worldMatrix, XMMatrixIdentity());
	this->worldBuffer = worldBuffer;
	this->worldStruct = worldStruct;
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->pivotPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotate = false;
	//Load Texture 
	loadTexture(gDevice, *texturePath);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* vertices->size();


	D3D11_BUFFER_DESC ibd;

	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(Vertex) * vertices->size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibdData;
	ibdData.pSysMem = &indices[0]; //vertex list here &vertices[0]?

	gDevice->CreateBuffer(&ibd, &ibdData, &indexBuffer);


	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = vertices->data();
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."

	gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);


}
void Model::loadTexture(ID3D11Device* gDevice, std::string filePath)
{
	

	
	//Convert filepath to wString
	if (filePath == "")
		filePath = "testTX.jpg";

	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());
	
	//Convert the wString to wchar_t* (Needed by the texture loader)
	const wchar_t* fileName = widestr.c_str();
	
	//load Texture
	HRESULT hr = CoInitialize((LPVOID)0);
	
	//The function will also create a subresource and bind it to the gpu
	hr= CreateWICTextureFromFile(gDevice, fileName, nullptr, &this->texture);


	//Create an error if texture is not loaded
	/*if (!SUCCEEDED(hr))
		MessageBox(*winHandle, L"Cannot intialize input device", L"Error", MB_OK);

	*/
	
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
	this->pivotPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//texture file name
	std::string textureFileName;
	std::vector<UINT> indices;

	ObjHandler* importer = new ObjHandler(&children,filePath,vertices, textureFileName,gDevice,gDeviceContext,worldBuffer,worldStruct,indices);//Make import

	//Load Texture 
	loadTexture(gDevice, textureFileName);
	

	delete importer; // delete when done;
	

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* vertices->size();

	D3D11_BUFFER_DESC ibd;

	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibdData;
	ibdData.pSysMem = indices.data(); //vertex list here &vertices[0]?

	gDevice->CreateBuffer(&ibd, &ibdData, &indexBuffer);
	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = vertices->data();
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."
	this->indicesCount = indices.size();
	gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	this->updateWorldMatrix();
	

//	HRESULT result;
	
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
	this->pivotPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);


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

void Model::setVertex(Vertex nVertex)
{
	this->vertices->push_back(nVertex);
}

void Model::sendToConstantBuffer()
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
	//Calculate offset from pivot to the translation
	XMFLOAT3 offset;

	offset.x = pivotPoint.x - translation.x;
	offset.y = pivotPoint.y - translation.y;
	offset.z = pivotPoint.z - translation.z;
		




	this->pivotPoint.x = newTranslation.x + offset.x;
	this->pivotPoint.y = newTranslation.y + offset.y;
	this->pivotPoint.z = newTranslation.z + offset.z;

	//PIVOT POINT MUST GET THE TRANSLATION AS WELL

	this->translation = newTranslation;
	updateWorldMatrix();
}

void Model::setRotation(XMFLOAT3 degrees)
{
	this->rotation.x += degrees.x;
	this->rotation.y += degrees.y;
	this->rotation.z += degrees.z;
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
	if (children != nullptr)
	{
		for (int i = 0; i < this->children->size(); i++)
		{
			delete this->children->at(i);


		}
		delete children;

	}
	if (vertices != nullptr)
	{
		delete vertices;
		this->vertexBuffer->Release();
	}
}

void Model::update()
{

	//This can be recursive, if the model has children.
	//they must be rendered first

	//BUT! this models transformation must be calculated first, so that the children wont be one frame behind
	//for example. the rotation must be done here first before we sent this(the parents) information to the children

	if (rotate)
	{
		
		this->setRotation(XMFLOAT3(0, rotationSpeed, 0));


	}

	//Do Frame transformations here

	

	//update  children, If there is any
	if (this->children != nullptr)
	{
		for (int i = 0; i < this->children->size(); i++) //maybe send in the worldStruct of the parent?
		{
			this->children->at(i)->update();

		}

	}


	

	


}

void Model::render()
{
	//update and render all the children
	
	if (this->children != nullptr)
	{
		renderChildren();

	}
	this->sendToConstantBuffer();
	this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer); 

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//IF there is a texture. apply it to the pixel shader

	if (texture != nullptr) {

		this->gDeviceContext->PSSetShaderResources(0, 1, &this->texture);
	}

	//fix draw indexed, first place should be the number of indices
	this->gDeviceContext->DrawIndexed(indicesCount, 0, 0);
	//this->gDeviceContext->Draw(this->vertices->size(), 0); //This will be dynamic,


}

void Model::renderChildren()
{
		//update and render all the children
	for (int i = 0; i < this->children->size(); i++)
	{
		this->children->at(i)->worldMatrix = this->worldMatrix;
		this->children->at(i)->normalWorld = this->normalWorld;
		this->children->at(i)->render();
	}


}

void Model::updateWorldMatrix()
{

	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	
	//We convert from degrees to radians here. Before this point we work in degrees to make it easier for the programmer and user
	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(toRadian(rotation.x));
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(toRadian(rotation.y));
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(toRadian(rotation.z));
	

	/*
		To rotate around the models pivot point.

		Translate to -pivot point, rotate*scale , translate back
	
	
	
	*/

	
	/*Get a vector from the translation to the pivot point */
	XMFLOAT3 vectorToTranslate;
	vectorToTranslate.x = pivotPoint.x - translation.x;
	vectorToTranslate.y = pivotPoint.y - translation.y;
	vectorToTranslate.z = pivotPoint.z - translation.z;

	

	
	//Create the rotation matrix
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
	rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);


	//Intoduce the world matrix, multiply rotation and scale. (world translation comes later)
	DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(rotationMatrix, scaleMatrix);
	
	//Translate the distance of the translation------>pivot (Inverted) vector
	DirectX::XMMATRIX pivotTranslation = DirectX::XMMatrixTranslation(vectorToTranslate.x*-1, vectorToTranslate.y*-1, vectorToTranslate.z* -1);
	
	//Multiply the rotation and scale matrix with this translation
	world = DirectX::XMMatrixMultiply(pivotTranslation,world);

	//Turn back the inverted vector and create a matrix that will translate back to origin
	pivotTranslation = DirectX::XMMatrixTranslation(vectorToTranslate.x, vectorToTranslate.y, vectorToTranslate.z);
	
	//Translate back to original position (origin)
	world = DirectX::XMMatrixMultiply(world, pivotTranslation);



	//Create the world translation matrix
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

	//Multiply the (scale*rotation) matrix with the world translation matrix
	world = DirectX::XMMatrixMultiply(world, translationMatrix);

	XMVECTOR worldDet = XMMatrixDeterminant(world);
	XMStoreFloat4x4(&normalWorld, XMMatrixInverse(&worldDet, world));
	
	
	world =XMMatrixTranspose(world);


	XMStoreFloat4x4(&this->worldMatrix, world);
}


