#include "SkyBox.h"
#include "DirectXToolkit.h"


/* 

	Read Skybox.h for description
*/

SkyBox::SkyBox()
{
	
}


SkyBox::~SkyBox()
{
}

bool SkyBox::Init(ID3D11DeviceContext* gDeviceContext, ID3D11Device *gDevice, ID3D11Buffer * worldBuffer, worldConstantBuffer* worldStruct)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	this->worldBuffer = worldBuffer;
	this->worldStruct = worldStruct;
	this->translation = XMFLOAT3(7.0f, 4.0f, 0.0f);
	
	HRESULT hr;
	this->vertices = new std::vector<Vertex>;
	
	if (!this->createCube())
	{//Create the cube, and if it fails, return false
		return false;
	};


	if (!this->createRenderStates())
		return false;
	if (!this->loadTexture(""))
		return false;
	return true;
}

bool SkyBox::createCube()
{


	//First tris
	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		0.5f,0.5f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 0.0f,   1.0f,
		1.0f,0.0f
	});




	//Second Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Third Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		0.0f,1.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		0.0f,1.0f
	});


	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		0.0f,1.0f
	});

	//Fourth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		0.0f,1.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		0.0f,1.0f
	});

	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		0.0f,1.0f
	});


	//Fifth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , -0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Sixth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , -0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Seventh Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , -0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});



	//Eighth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , -0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Ninth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, 0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});



	//Tenth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});



	//Eleventh Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Twelfth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		-0.5f, -0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});


	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
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
	HRESULT hr;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
		return false;
	else
		return true;
	
}

bool SkyBox::loadTexture(std::string filePath)
{

	//Convert filepath to wString
	if (filePath == "")
		filePath = "sunsetcube1024.dds";

	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());

	//Convert the wString to wchar_t* (Needed by the texture loader)
	const wchar_t* fileName = widestr.c_str();

	//load Texture
	HRESULT hr = CoInitialize((LPVOID)0);

	

	hr = CreateDDSTextureFromFile(gDevice, fileName,nullptr, &this->texture);
	
	if (FAILED(hr))
		return false;

	this->gDeviceContext->PSSetShaderResources(1, 1, &this->texture);

	
	return true;
}

bool SkyBox::createRenderStates()

{
	HRESULT hr;
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;

	hr = gDevice->CreateRasterizerState(&cmdesc, &RSCullNone);
	if (FAILED(hr))
		return false;
	
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr= gDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);
	if (FAILED(hr))
		return false;
	return true;

}


void SkyBox::render()
{
	/*this->gDeviceContext->VSSetShader(SKYMAP_VS, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(SKYMAP_GS, nullptr, 0);
	this->gDeviceContext->PSSetShader(SKYMAP_PS, nullptr, 0);
	this->gDeviceContext->IASetInputLayout(gInputLayout);
*/

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//IF there is a texture. apply it to the pixel shader

	if (texture != nullptr) {
		
		this->gDeviceContext->PSSetShaderResources(1, 1, &this->texture);
	}


	this->gDeviceContext->Draw(this->vertices->size(), 0); //This will be dynamic,




}


void SkyBox::update(XMFLOAT3 camPos)

{
	XMMATRIX test = XMMatrixMultiply(XMMatrixScaling(100, 100, 100), XMMatrixTranslation(camPos.x, camPos.y, camPos.z));
	XMStoreFloat4x4(&this->worldMatrix, XMMatrixTranspose(test));

	this->worldStruct->world = this->worldMatrix;
	this->worldStruct->normalWorld = this->worldMatrix;


	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	worldConstantBuffer* temporaryWorld = (worldConstantBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	this->gDeviceContext->Unmap(worldBuffer, 0);

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer);

	//set the rasterizer state and depth buffer state for the skybox
	gDeviceContext->OMSetDepthStencilState(DSLessEqual, 0);
	gDeviceContext->RSSetState(RSCullNone);

}

void SkyBox::Release()
{
	/*if (SKYMAP_VS != nullptr)
		SKYMAP_VS->Release();
	if (SKYMAP_GS != nullptr)
		SKYMAP_GS->Release();
	if (SKYMAP_PS != nullptr)
		SKYMAP_PS->Release();*/

	if (texture != nullptr)
		texture->Release();
	if (vertexBuffer != nullptr)
		vertexBuffer->Release();
	if (vertices != nullptr)
		delete vertices;
	
	//no need to check i realized here. I do a check at initalization, if there is none of these things, the error will be shown att initalization
	/*SKYMAP_VS->Release();
	SKYMAP_PS->Release();

	*/
	
	DSLessEqual->Release();
	RSCullNone->Release();
	
}
