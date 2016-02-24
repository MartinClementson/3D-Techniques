#pragma once

#include "Linker.h"
using namespace DirectX;


class Frustum
{

private:

	struct PLANE
	{
		float Distance;
		XMFLOAT3 Normal;
	};

	PLANE planes[6];

	
public:
	Frustum();
	~Frustum();
	void ConstructFrustum(float screenDepth, XMFLOAT4X4 &projectionMatrix, XMFLOAT4X4 &viewMatrix);


	//Check functions

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
};

