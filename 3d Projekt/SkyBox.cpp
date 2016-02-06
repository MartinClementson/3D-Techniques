#include "SkyBox.h"



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
	XMMATRIX test= XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(7.0, 2.0, 0.0));
	XMStoreFloat4x4(&this->worldMatrix, XMMatrixTranspose(test));
	HRESULT hr;
	this->vertices = new std::vector<Vertex>;
	
	if (!this->createCube())
	{//Create the cube, and if it fails, return false
		return false;
	};
	if (!this->loadShaders())
	{
		return false;
	};
	return true;
}

bool SkyBox::loadShaders()
{
	HRESULT hr;
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"VertexShaderSky.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &SKYMAP_VS);

	if (FAILED(hr))
		return false;


	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		/*{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0 }*/ //not in use
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA,0 } //We wont use Color here, that's why the offset is 32. were still using the same struct
																						//Normals?

	};
	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gInputLayout);
	pVS->Release();
	if (FAILED(hr))
		return false;

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderSky.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &SKYMAP_GS);
	pGS->Release();

	if (FAILED(hr))
		return false;









	//Pixel shader
	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"PixelShaderSky.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &SKYMAP_PS);
	pPs->Release();
	if (FAILED(hr))
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
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 0.0f,   1.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
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
		//Second Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Third Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});


	//Fourth Tris

	this->vertices->push_back(Vertex
	{
		//first Vert
		0.5f, 0.5f , 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
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
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});
	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , -0.5f,
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
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f,
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
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, -0.5f , 0.5f,
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
		//Second Vert
		-0.5f, -0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , 0.5f,
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
		//Second Vert
		0.5f, 0.5f , 0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, 0.5f , -0.5f,
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
		//Second Vert
		0.5f, 0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		-0.5f, 0.5f , -0.5f,
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
		//Second Vert
		0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , 0.5f,
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
		//Second Vert
		-0.5f, -0.5f , -0.5f,
		0.0f, 1.0f,   0.0f,
		1.0f,0.0f
	});

	this->vertices->push_back(Vertex
	{
		//Third Vert
		0.5f, -0.5f , -0.5f,
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





void SkyBox::render()
{
	this->gDeviceContext->VSSetShader(SKYMAP_VS, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(SKYMAP_GS, nullptr, 0);
	this->gDeviceContext->PSSetShader(SKYMAP_PS, nullptr, 0);
	this->gDeviceContext->IASetInputLayout(gInputLayout);


	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//IF there is a texture. apply it to the pixel shader

	//if (texture != nullptr) {

		//this->gDeviceContext->PSSetShaderResources(0, 1, &this->texture);
	//}


	this->gDeviceContext->Draw(this->vertices->size(), 0); //This will be dynamic,




}


void SkyBox::update()

{
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

}

void SkyBox::Release()
{
	if (SKYMAP_VS != nullptr)
		SKYMAP_VS->Release();
	if (SKYMAP_GS != nullptr)
		SKYMAP_GS->Release();
	if (SKYMAP_PS != nullptr)
		SKYMAP_PS->Release();
	if (skyTexture != nullptr)
		skyTexture->Release();
	if (vertexBuffer != nullptr)
		vertexBuffer->Release();
	if (vertices != nullptr)
		delete vertices;
	if (gInputLayout != nullptr)
		gInputLayout->Release();

	//no need to check i realized here. I do a check at initalization, if there is none of these things, the error will be shown att initalization
	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	
	DSLessEqual->Release();
	//RSCullNone->Release();

}
