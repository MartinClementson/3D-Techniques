#include "Camera.h"

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, float b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;

	return result;
}

DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}


Camera::Camera()
{
	//creating basic View parameters
	viewPosition = { 0.0f, 0.0f, 4.0f };
	viewLookAt = { 0.0f, 0.0f, -1.0f };
	viewRightDirection = { 1.0f, 0.0f, 0.0f };
	viewUpDirection = { 0, 1.0f, 0 };
	yaw = 0;
	pitch = 0;
	//creating the projection matrix
	DirectX::XMMATRIX tempProjection;
	tempProjection = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI*0.45f), (WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 10000.0f);
	tempProjection = DirectX::XMMatrixTranspose(tempProjection);
	DirectX::XMStoreFloat4x4(&projection, tempProjection);
	

}

Camera::~Camera()
{
}

void Camera::setViewPosition(DirectX::XMFLOAT3& nViewPos)
{
	this->viewPosition = nViewPos;
}

void Camera::setViewLookAt(DirectX::XMFLOAT3& nViewLookAt)
{
	
	this->viewLookAt = nViewLookAt;
}

void Camera::rotateYaw(float angle)
{
	yaw += toRadian(angle);
	
	DirectX::XMMATRIX yawMatrix;
	yaw = min(yaw, PI* 0.49); // Almost fully up
	yaw = max(yaw, PI * -0.49); // Almost fully down
	//The D3DXMatrixRotationAxis function takes a vector and an angle and produces a matrix that will provide that requested rotation.
	yawMatrix = DirectX::XMMatrixRotationY(yaw);
	//DirectX::XMMatrixRotationRollPitchYaw()

	//this->viewLookAt = DirectX::XMVector3TransformCoord(this->viewLookAt, yawMatrix);

	/*if (angle > 360)
	{
		angle = 0;

	}
	else if (angle < -360)
	{
		angle = 0;
	}*/



}

void Camera::rotatePitch(float angle)
{
	pitch += toRadian(angle);
	pitch = fmod(pitch, 2 * PI); // Keep in valid circular range
	pitch = min(pitch, PI* 0.49); // Almost fully up
	pitch = max(pitch, PI * -0.49); // Almost fully down
	//if (pitch > 0.523598776)
	//{
	//	pitch = 0.523598776;
	//}
	//if (pitch < -0.523598776)
	//{
	//	pitch = -0.523598776;
	//}

	//storing the view matrix variables in a new matrix

	DirectX::XMMATRIX pitchMatrix;
	//pitchMatrix = DirectX::XMMatrixRotationX(pitch);
	pitchMatrix=DirectX::XMMatrixRotationRollPitchYaw(pitch, 0.0f, 0.0f);
	this->viewLookAt = { 0.0f,0.0f,-1.0f };
	

}

DirectX::XMFLOAT4X4 Camera::getView()
{
	
	XMVECTOR R = XMLoadFloat3(&viewRightDirection);
	XMVECTOR U = XMLoadFloat3(&viewUpDirection);
	XMVECTOR L = XMLoadFloat3(&viewLookAt);
	XMVECTOR P = XMLoadFloat3(&viewPosition);

	//Normalize the L vector
	L = XMVector3Normalize(L);

	//Compute a new corrected "up" vector and normalize it!
	//Using cross product of Look and Right vectors
	U = XMVector3Normalize(XMVector3Cross(L, R));

	//Compute a new corrected "Right" vector
	//U and L are already ortho-normal, so no need to normalize it;
	R = XMVector3Cross(U, L);

	XMStoreFloat3(&viewRightDirection, R);
	XMStoreFloat3(&viewUpDirection, U);
	XMStoreFloat3(&viewLookAt, L);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));
	//Manually create the view matrix
	XMFLOAT4X4 view;
	view(0, 0) = viewRightDirection.x;
	view(1, 0) = viewRightDirection.y;
	view(2, 0) = viewRightDirection.z;
	view(3, 0) = x;

	view(0, 1) = viewUpDirection.x;
	view(1, 1) = viewUpDirection.x;
	view(2, 1) = viewUpDirection.x;
	view(3, 1) = y;
	
	view(0, 2) = viewLookAt.x;
	view(1, 2) = viewLookAt.x;
	view(2, 2) = viewLookAt.x;
	view(3, 2) = z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;

	//CREATE A MATRIX TO TRANSPOSE HERE!! 


	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
	//	(
	//		DirectX::XMLoadFloat3(&viewPosition),
	//		DirectX::XMLoadFloat3(&viewLookAt),
	//		DirectX::XMLoadFloat3(&viewUpDirection)
	//	);

	//this->viewLookAt = { 0.0f,0.0f,1.0f };

	//converting the view matrix into an XMFLOAT4X4, for simpler use
	//viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	//DirectX::XMFLOAT4X4 returnView;
	//DirectX::XMStoreFloat4x4(&returnView, viewMatrix);

	return view;
}

DirectX::XMFLOAT4X4 Camera::getProjection()
{
	return projection;
}

DirectX::XMFLOAT3 Camera::getCamPos()
{
	
	return this->viewPosition;
	
}

void Camera::move(moveDirection direction)
{
	;
	switch (direction)
	{
	case FORWARD:
	{
		viewPosition +=  viewLookAt * CAMERA_SPEED;
		//Replicates a floating-point value into all four components of a vector.
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(CAMERA_SPEED); //speed
		DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
		DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position
		XMStoreFloat3(&viewPosition, XMVectorMultiplyAdd(s, l, p));

		break;
	}
	case BACKWARD:
	{
		viewPosition += viewLookAt * -CAMERA_SPEED;
		//Replicates a floating-point value into all four components of a vector.
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(-CAMERA_SPEED); //speed
		DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
		DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position
		XMStoreFloat3(&viewPosition, XMVectorMultiplyAdd(s, l, p));

		
		break;
	}
	case RIGHT:
	{
		viewPosition += viewRightDirection * CAMERA_SPEED;
		XMVECTOR s = XMVectorReplicate(CAMERA_SPEED);
		XMVECTOR r = XMLoadFloat3(&viewRightDirection);
		XMVECTOR p = XMLoadFloat3(&viewPosition);
		XMStoreFloat3(&viewPosition, XMVectorMultiplyAdd(s, r, p));
		break;

	}
	case LEFT:
	{

		viewPosition += viewRightDirection * -CAMERA_SPEED;
		XMVECTOR s = XMVectorReplicate(-CAMERA_SPEED);
		XMVECTOR r = XMLoadFloat3(&viewRightDirection);
		XMVECTOR p = XMLoadFloat3(&viewPosition);
		XMStoreFloat3(&viewPosition, XMVectorMultiplyAdd(s, r, p));
		break;
		
		break;

	
	}

	}


}
