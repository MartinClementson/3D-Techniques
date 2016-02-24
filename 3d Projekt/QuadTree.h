#pragma once
#include "Linker.h"
#include "Terrain.h"
//include frustrum here when it's done

class QuadTree
{
private:
	struct NodeType
	{
		float posX, posY, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};
public:
	QuadTree();
	QuadTree(const QuadTree &parent);
	~QuadTree();

	bool Initialize(Terrain *terrain, ID3D11Device *gDevice);
	void Release();
	void render(ID3D11DeviceContext *gDeviceContext, Terrain *terrain); //frustrumClass* supposed to be here too, also terrainShaderClass??

	int GetDrawCount();
};

