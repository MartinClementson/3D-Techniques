#include "Frustum.h"



Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, XMFLOAT4X4 &projectionMatrix, XMFLOAT4X4 &viewMatrix)
{
	//This is called every frame

	float zMinimum, r;
	XMMATRIX matrixTemp; //Needed to multiply

	//Calculate the minimum z distance in the frustum.
	zMinimum = -projectionMatrix._14 + projectionMatrix._33;
	r = screenDepth / (screenDepth -  zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r*zMinimum;

	//Create the frustum matrix from the view and projection matrix
	
	matrixTemp = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix));

	XMFLOAT4X4 M;
	XMStoreFloat4x4(&M, matrixTemp); //4x4 needed to be able to access individual components

	//Calculate Left plane of frustum
	planes[0].Normal.x = -(M._14 + M._11);
	planes[0].Normal.y = -(M._24 + M._21);
	planes[0].Normal.z = -(M._34 + M._31);
	planes[0].Distance = -(M._44 + M._41);

	//Right Clipping plane
	planes[1].Normal.x = -(M._14 - M._11);
	planes[1].Normal.y = -(M._24 - M._21);
	planes[1].Normal.z = -(M._34 - M._31);
	planes[1].Distance = -(M._44 - M._41);


	//Top Clipping plane
	planes[2].Normal.x = -(M._14 - M._12);
	planes[2].Normal.y = -(M._24 - M._22);
	planes[2].Normal.z = -(M._34 - M._32);
	planes[2].Distance = -(M._44 - M._42);

	//Bottom clipping plane
	planes[3].Normal.x = -(M._14 + M._12);
	planes[3].Normal.y = -(M._24 + M._22);
	planes[3].Normal.z = -(M._34 + M._32);
	planes[3].Distance = -(M._44 + M._42);

	//Near Clipping plane
	planes[4].Normal.x = -(M._14 + M._13);
	planes[4].Normal.y = -(M._24 + M._23);
	planes[4].Normal.z = -(M._34 + M._33);
	planes[4].Distance = -(M._44 + M._43);

	//Far Clipping Plane
	planes[5].Normal.x = -(M._14 - M._13);
	planes[5].Normal.y = -(M._24 - M._23);
	planes[5].Normal.z = -(M._34 - M._33);
	planes[5].Distance = -(M._44 - M._43);
	

	//Normalize all the planes
	//1/length of normal * vector.x.y.z 
	for (int i = 0; i < 6; i++)
	{	//Magnitude of vector == 1/length of normal
		
		float lengthSquared =
			planes[i].Normal.x * planes[i].Normal.x +
			planes[i].Normal.y * planes[i].Normal.y +
			planes[i].Normal.z * planes[i].Normal.z;

		float denom = 1.0f / sqrt(lengthSquared);
		
		planes[i].Normal.x *= denom;
		planes[i].Normal.y *= denom;
		planes[i].Normal.z *= denom;
		planes[i].Distance *= denom;

	}




}

bool Frustum::CheckPoint(float x, float y, float z)
{
	return false;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	return false;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	return false;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	return false;
}
