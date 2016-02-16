#include "Terrain.h"
#include <fstream>


void Terrain::Release()
{
	heightMapRSV->Release();
}

Terrain::Terrain()
{
	heightMapHeight = 2049;
	heightMapWidth = 2049;
	heightScale = 50;
}

bool Terrain::inBounds(int i, int j)
{
	return true;
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
	std::vector<unsigned char> in(heightMapHeight * heightMapWidth);
	std::ifstream loading;
	loading.open("terrain.raw", std::ios_base::binary);
	if (!loading)
		return false;
	else
	{
		loading.read((char*)&in[0], (std::streamsize)in.size());

		loading.close();
	}
	mHeightMap.resize((heightMapHeight * heightMapWidth), 0);
	for (UINT i = 0; i < (heightMapHeight * heightMapWidth); ++i)
	{

		mHeightMap.at(i) = (in.at(i) / 255.0f)*heightScale;
	}
	return true;
}

Terrain::~Terrain()
{
}
