#include "Overlay.h"



void Overlay::loadTexture(ID3D11Device * gDevice, std::string filename)
{

	//Convert filepath to wString
	if (filename == "")
		filename = "overlay.png";

	std::wstring widestr = std::wstring(filename.begin(), filename.end());

	//Convert the wString to wchar_t* (Needed by the texture loader)
	const wchar_t* fileName = widestr.c_str();

	//load Texture
	HRESULT hr = CoInitialize((LPVOID)0);

	//The function will also create a subresource and bind it to the gpu
	hr = CreateWICTextureFromFile(gDevice, fileName, nullptr, &this->texture);


	this->gDeviceContext->PSSetShaderResources(5, 1, &this->texture);
}

Overlay::Overlay(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->gDeviceContext = gDeviceContext;


	this->loadTexture(gDevice, "");



	point.x = 0.0f;
	point.y = 0.0f;
	point.z = 0.0f; //Create one point in the center. (We make it a quad in the GS)



	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex);


	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = &point;
	// data() "Returns a direct pointer to the memory array used internally by the vector to store its owned elements."

	gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);


}

void Overlay::Release()
{
	this->vertexBuffer->Release();
	this->texture->Release();
}

void Overlay::Render()
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	this->gDeviceContext->Draw(1, 0);


}


Overlay::~Overlay()
{
}
