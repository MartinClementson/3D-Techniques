#include "Camera.h"



Camera::Camera()
{
	//creating basic View parameters
	viewPosition = { 0.0f, 0.0f, 4.0f };
	viewLookAt = { 0.0f, 0.0f, 0.0f };
	viewUpDirection = { 0, 1.0f, 0 };

	//creating the projection matrix
	DirectX::XMMATRIX tempProjection;
	tempProjection = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI*0.45f), (WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 10000.0f);
	tempProjection = DirectX::XMMatrixTranspose(tempProjection);
	DirectX::XMStoreFloat4x4(&projection, tempProjection);
	

}

Camera::~Camera()
{
}

void Camera::setViewPosition(DirectX::XMVECTOR nViewPos)
{
	this->viewPosition = nViewPos;
}

void Camera::setViewLookAt(DirectX::XMVECTOR nViewLookAt)
{
	this->viewLookAt = nViewLookAt;
}

DirectX::XMFLOAT4X4 Camera::getView()
{
	//storing the view matrix variables in a new matrix
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
		(
			viewPosition,
			viewLookAt,
			viewUpDirection
		);

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