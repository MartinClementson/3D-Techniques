#include "Frustum.h"



Frustum::Frustum()
{
	for (int i = 0; i < 6; i++)
	{
		planes[i].Normal.x = 0.0f;
		planes[i].Normal.y = 0.0f;
		planes[i].Normal.z = 0.0f;

		planes[i].Distance = 0.0f;
	}
	
}


Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 &viewMatrix)
{
	//This is called every frame

	float zMinimum, r;
	XMMATRIX matrixTemp; //Needed to multiply

	//Calculate the minimum z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth -  zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r*zMinimum;

	//Create the frustum matrix from the view and projection matrix
	
	matrixTemp = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix));

	XMMatrixTranspose(matrixTemp);// <--- maybe transpose????!?!

	XMFLOAT4X4 M;
	XMStoreFloat4x4(&M, matrixTemp); //4x4 needed to be able to access individual components

	//Calculate Left plane of frustum
	planes[0].Normal.x = (M._14 + M._11);
	planes[0].Normal.y = (M._24 + M._21);
	planes[0].Normal.z = (M._34 + M._31);
	planes[0].Distance = (M._44 + M._41);

	//Right Clipping plane
	planes[1].Normal.x = (M._14 - M._11);
	planes[1].Normal.y = (M._24 - M._21);
	planes[1].Normal.z = (M._34 - M._31);
	planes[1].Distance = (M._44 - M._41);


	//Top Clipping plane
	planes[2].Normal.x = (M._14 - M._12);
	planes[2].Normal.y = (M._24 - M._22);
	planes[2].Normal.z = (M._34 - M._32);
	planes[2].Distance = (M._44 - M._42);

	//Bottom clipping plane
	planes[3].Normal.x = (M._14 + M._12);
	planes[3].Normal.y = (M._24 + M._22);
	planes[3].Normal.z = (M._34 + M._32);
	planes[3].Distance = (M._44 + M._42);

	//Near Clipping plane
	planes[4].Normal.x = (M._14 + M._13);
	planes[4].Normal.y = (M._24 + M._23);
	planes[4].Normal.z = (M._34 + M._33);
	planes[4].Distance = (M._44 + M._43);

	//Far Clipping Plane
	planes[5].Normal.x = (M._14 - M._13);
	planes[5].Normal.y = (M._24 - M._23);
	planes[5].Normal.z = (M._34 - M._33);
	planes[5].Distance = (M._44 - M._43);
	

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
	int i;

	//Check if the point is inside all six planes of the frustum

	for (i = 0; i < 6; i++)
	{
		/*
			 When doing checks we put the point into the planes equation,
			 Ax+By+Cz+D

			 the normal is A,B,C
			 the distance is D

			 we need to put these together to do the test.

			 We put them in a float3. then we convert that to a XM vector, so that we may use the XMplaneDot function
		
		From microsoft:

		XMPlaneDotCoord(P,V)
		Returns the dot product between P and V replicated into each of the four components of the returned XMVECTOR.
		
		*/

		XMFLOAT4 plane =
		{   
			planes[i].Normal.x,
			planes[i].Normal.y,
			planes[i].Normal.z,
			planes[i].Distance
		};

		XMFLOAT3 point = { x, y, z };
		
		//XMLoadFloat4 converts the type from XMFLOAT4 to XMVECTOR
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] < 0.0f) //If the point is outside any plane, Return false
		{
			return false;

		}
	}
	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{

	int i;
	//check if any point of the cube is in the view frustum

	for (i = 0; i < 6; i++)
	{


		XMFLOAT4 plane =
		{
			planes[i].Normal.x,
			planes[i].Normal.y,
			planes[i].Normal.z,
			planes[i].Distance
		};

		XMFLOAT3 point = { (xCenter-radius), (yCenter-radius), (zCenter-radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + radius), (yCenter - radius), (zCenter - radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - radius), (yCenter + radius), (zCenter - radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + radius), (yCenter + radius), (zCenter - radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - radius), (yCenter - radius), (zCenter + radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + radius), (yCenter - radius), (zCenter + radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - radius), (yCenter + radius), (zCenter + radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + radius), (yCenter + radius), (zCenter + radius)};
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{


	int i;
	//check if the radius of the sphere from the center point is inside all six planes
	//if it is outside any of them, return false as it cannot be seen.

	XMFLOAT3 centerPoint = { xCenter , yCenter , zCenter};

	//Inser the spheres center into each plane equation to get the distance

	//if distance  < -radius then it's outside
	for (i = 0; i < 6; i++)
	{


		XMFLOAT4 plane =
		{
			planes[i].Normal.x,
			planes[i].Normal.y,
			planes[i].Normal.z,
			planes[i].Distance
		};

		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&centerPoint)).m128_f32[0] < -radius)
			return false;

	}
	return true;
}


// This Function is Similar to CheckCube.
// But we send in a radius for x,y,z. And calculate the 8 corner points of the rectangle
// (CheckCube only had one radius)
bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	
	
	
	int i;
	//check if any point of the cube is in the view frustum

	for (i = 0; i < 6; i++)
	{


		XMFLOAT4 plane =
		{
			planes[i].Normal.x,
			planes[i].Normal.y,
			planes[i].Normal.z,
			planes[i].Distance
		};

		XMFLOAT3 point = { (xCenter - xSize), (yCenter - ySize), (zCenter - zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + xSize), (yCenter - ySize), (zCenter - zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - xSize), (yCenter + ySize), (zCenter - zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + xSize), (yCenter + ySize), (zCenter - zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - xSize), (yCenter - ySize), (zCenter + zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + xSize), (yCenter - ySize), (zCenter + zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter - xSize), (yCenter + ySize), (zCenter + zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		point = { (xCenter + xSize), (yCenter + ySize), (zCenter + zSize) };
		if (XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&point)).m128_f32[0] >= 0.0f)
			continue;

		return false;
	}

	return true;
}
