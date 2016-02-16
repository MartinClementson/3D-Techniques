#include "Terrain.h"
#include <fstream>


void Terrain::release()
{
	mHeightMap->Release();
}

Terrain::Terrain()
{
}

bool Terrain::inBounds(int i, int j)
{

}

float Terrain::average(int i, int j)
{
	return 0.0f;
}

void Terrain::smooth()
{
}

bool Terrain::init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext)
{
	HRESULT hr;
	std::vector<unsigned char> in;
	std::ifstream loading;
	loading.open(fileName);
	if (!loading)
		return false;
	else
	{
		loading.read((char*)&in[0], (std::streamsize)in.size());
	}
	return hr;
}

Terrain::~Terrain()
{
}
