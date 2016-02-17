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
	// true if ij are valid indices, false ortherwise

	return
		i >= 0 && i < (int)heightMapHeight &&
		j >= 0 && j < (int)heightMapWidth;
	
}

float Terrain::average(int i, int j)
{
	/*This function computes the average height oj the ij element.
	It does this by averaging with 8 neighbour pixel, If we are at the edges, we take as many as possible*/
	
	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg += mHeightMap[m * heightMapWidth + n];
				num += 1;

			}

		}

	}
	
	
	return avg/num;
}

void Terrain::smooth()
{
	std::vector<float> dest(mHeightMap.size());
	for (UINT i = 0; i < heightMapHeight; i++)
	{

		for (UINT j = 0; j < heightMapWidth; j++)
		{

			dest[i * heightMapWidth + j] = average(i, j);

		}

	}
	//replace the old heightmap with the new filtered one
	mHeightMap = dest;

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
