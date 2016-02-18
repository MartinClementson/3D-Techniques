#pragma once
#include "Linker.h"


// http://www.rastertek.com/tertut02.html
class Terrain
{
private:
	float heightScale;
	float cellSpacing;
	UINT heightMapWidth;
	UINT heightMapHeight;

	//from the tutorial
	struct HeightMapType
	{
		float x, y, z;
	};
	UINT m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_HeightMap;
	int index;
	Vertex* vertices;
	unsigned long* indices;

	//a different tutorial
	int NumVertices;
	int NumFaces;

	std::vector<Vertex> hMapPoints;
	std::vector<float> mHeightMap;
	bool inBounds(int i, int j);
	float average(int i, int j);
	void smooth();
	void createPoints();
	ID3D11ShaderResourceView *heightMapRSV = nullptr;

public:

	bool initializeBuffers(ID3D11Device *gDevice);
	void Release();
	void Render(ID3D11DeviceContext *gDeviceContext);
	Terrain();
	bool init(std::string fileName, ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext);
	std::vector<Vertex> getPoints() { return hMapPoints; }
	~Terrain();
	int getIndexCount() { return m_indexCount; }
};

