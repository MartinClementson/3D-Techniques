#pragma once

#include "Linker.h"
using namespace DirectX;


// IMPORTANT!!
// This frustum is build up with having the normals of the planes turned inwards.
// Some places shows how to test with the normals inwards and some with the normals outwards.
// Keep this in mind when reading this code (It differs from the lecture example)

class Frustum
{


private:
	struct PLANE
	{
		float Distance;
		XMFLOAT3 Normal;
		//Ax+By+Cz+D =0
	};

	PLANE planes[6];

	
public:
	Frustum();
	~Frustum();
	void ConstructFrustum(float screenDepth, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 &viewMatrix);


	//Check functions

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
};

