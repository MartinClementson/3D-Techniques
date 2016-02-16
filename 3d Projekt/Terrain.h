#pragma once
#include "Linker.h"

class Terrain
{
private:
	float heightScale;
	float cellSpacing;
	UINT heightMapWidth;
	UINT heightMapHeight;
	std::vector<float> mHeightMap;
	bool inBounds(int i, int j);
	float average(int i, int j);
	void smooth();

	ID3D11ShaderResourceView *mHeightMap = nullptr;
public:
	void release();
	Terrain();
	bool init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext);
	~Terrain();
};

