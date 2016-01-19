#include "Camera.h"



Camera::Camera()
{
	//creating basic View parameters
	viewPosition = { 0.0f, 0.0f, 4.0f };
	viewLookAt = { 0.0f, 0.0f, -1.0f };
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
	this->viewPosition = DirectX::XMLoadFloat3(&nViewPos);
}

void Camera::setViewLookAt(DirectX::XMFLOAT3& nViewLookAt)
{
	DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&nViewLookAt);

	this->viewLookAt = temp;
}

void Camera::rotateYaw(float angle)
{
	yaw += angle;

	DirectX::XMMATRIX yawMatrix;
	//The D3DXMatrixRotationAxis function takes a vector and an angle and produces a matrix that will provide that requested rotation.
	yawMatrix = DirectX::XMMatrixRotationY(yaw);

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
	pitch = angle;
	if (angle > 87)
	{
		angle = 0;
	}
	else if (angle < -87)
	{
		angle = 0;
	}

	//storing the view matrix variables in a new matrix

	DirectX::XMMATRIX pitchMatrix;
	pitchMatrix = DirectX::XMMatrixRotationX(pitch);

	this->viewLookAt = DirectX::XMVector3Transform(viewLookAt, pitchMatrix);

}

DirectX::XMFLOAT4X4 Camera::getView()
{
	
	//this->viewLookAt = DirectX::XMVector3Normalize(viewLookAt);
	//this->viewUpDirection = DirectX::XMVector3TransformCoord(viewUpDirection, pitchMatrix);
	//this->viewUpDirection = DirectX::XMVector3Normalize(viewUpDirection);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
		(
			viewPosition,
			viewLookAt,
			viewUpDirection
		);

	//this->viewLookAt = { 0.0f,0.0f,1.0f };

	//converting the view matrix into an XMFLOAT4X4, for simpler use
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	DirectX::XMFLOAT4X4 returnView;
	DirectX::XMStoreFloat4x4(&returnView, viewMatrix);

	return returnView;
}

DirectX::XMFLOAT4X4 Camera::getProjection()
{
	return projection;
}

DirectX::XMFLOAT3 Camera::getCamPos()
{
	
		return DirectX::XMFLOAT3(this->viewPosition.m128_f32[0], this->viewPosition.m128_f32[1], this->viewPosition.m128_f32[2]);
	
}
