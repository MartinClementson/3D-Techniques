#include "Terrain.h"
#include <fstream>


void Terrain::Release()
{
	heightMapRSV->Release();
}

Terrain::Terrain()
{
	heightMapHeight = 1024;
	heightMapWidth = 1024;
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

void Terrain::createPoints()
{
	float height = heightMapHeight / 2;
	float width = heightMapWidth / 2;
	Vertex point;

	for (int j = 0; j < (heightMapHeight-1); j++ )
	{
		for (int i = 0; i < (heightMapWidth-1); i++)
		{
			int index1 = (heightMapHeight * j) + i;				//bottom left
			int index2 = (heightMapHeight * j) + (i + 1);		//Bottom Right
			int index3 = (heightMapHeight * (j + 1)) + i;		//Upper Left
			int index4 = (heightMapHeight * (j + 1)) + (i + 1); //Upper Right

			//upper left
			point.x = j + 1 + i;
			point.z = j + i;
			point.y = mHeightMap[index3];

			point.u = mHeightMap[index3];
			point.v = mHeightMap[index3];

			hMapPoints.push_back(point);

			//upper Right
			point.x = j + 1 + i;
			point.z = j + 1 + i;
			point.y = mHeightMap[index4];

			point.u = mHeightMap[index4];
			point.v = mHeightMap[index4];

			hMapPoints.push_back(point);

			//bottom Right
			point.x = j + i;
			point.z = j + 1 + i;
			point.y = mHeightMap[index2];

			point.u = mHeightMap[index2];
			point.v = mHeightMap[index2];

			hMapPoints.push_back(point);

			//bottom Right
			point.x = j + i;
			point.z = j + 1 + i;
			point.y = mHeightMap[index2];

			point.u = mHeightMap[index2];
			point.v = mHeightMap[index2];

			hMapPoints.push_back(point);

			//Bottom left
			point.x = j + i;
			point.z = j + i;
			point.y = mHeightMap[index1];

			point.u = mHeightMap[index1];
			point.v = mHeightMap[index1];

			hMapPoints.push_back(point);

			//upper left
			point.x = j + 1 + i;
			point.z = j + i;
			point.y = mHeightMap[index3];

			point.u = mHeightMap[index3];
			point.v = mHeightMap[index3];

			hMapPoints.push_back(point);

		}
	}
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
	smooth();
	return true;
}

Terrain::~Terrain()
{
}
