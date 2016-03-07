#pragma once
#include "Linker.h"
#include "DirectXToolkit.h"
class Overlay
{

private:
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Buffer* vertexBuffer = nullptr;
	Vertex point;
	ID3D11ShaderResourceView* texture = nullptr;
	Overlay();
public:
	void loadTexture(ID3D11Device* gDevice, std::string filename );
	Overlay(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void Release();
	void Render();
	~Overlay();
};

