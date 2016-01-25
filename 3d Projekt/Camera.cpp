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
DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}
DirectX::XMFLOAT3 operator-=(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}


Camera::Camera()
{
	
	//creating basic View parameters
	viewPosition = { 0.0f, 0.0f, -3.0f };
	viewLookAt = { 0.0f, 0.0f, 1.0f };
	viewRightDirection = { 1.0f, 0.0f, 0.0f };
	viewUpDirection = { 0, 1.0f, 0 };
	updateView();
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
	
	XMMATRIX yawMatrix = XMMatrixRotationY(toRadian(angle));

	XMStoreFloat3(&viewRightDirection, XMVector3TransformNormal(XMLoadFloat3(&viewRightDirection), yawMatrix));

	XMStoreFloat3(&viewUpDirection, XMVector3TransformNormal(XMLoadFloat3(&viewUpDirection), yawMatrix));

	XMStoreFloat3(&viewLookAt, XMVector3TransformNormal(XMLoadFloat3(&viewLookAt), yawMatrix));
	this->updateView();
}


void Camera::rotatePitch(float angle)
{
	float tempPitch = toRadian(angle);
	//pitch += tempPitch;
	//tempPitch = fmod(tempPitch, 2 * PI); // Keep in valid circular range
	//tempPitch = min(tempPitch, PI* 0.49); // Almost fully up
	//tempPitch = max(tempPitch, PI * -0.49); // Almost fully down
	/*if (pitch > 0.02)
	{
		
		pitch = 0.02;
		return;
	}
	if (pitch < -0.02)
	{
		pitch = -0.02;
		return;
	}*/

	//storing the view matrix variables in a new matrix

	
	XMMATRIX pitchMatrix = XMMatrixRotationAxis(XMLoadFloat3(&viewRightDirection), angle);

	XMStoreFloat3(&viewLookAt, XMVector3TransformCoord(XMLoadFloat3(&viewLookAt), pitchMatrix));


	//XMStoreFloat3(&this->viewUpDirection, XMVector3TransformCoord(XMLoadFloat3(&viewUpDirection), pitchMatrix));
	//this->viewLookAt = { 0.0f,0.0f,-1.0f };
	this->updateView();
	

}

DirectX::XMFLOAT4X4 Camera::getView()
{
	
	
	return this->view;
}

DirectX::XMFLOAT4X4 Camera::getProjection()
{
	return projection;
}

DirectX::XMFLOAT3 Camera::getCamPos()
{
	
	return this->viewPosition;
	
}

void Camera::walk(float d)
{

	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d); //speed
	DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
	DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position


											
	XMVECTOR normLook = XMVectorSubtract(l, p); //create a vector from the "look at" point ant the camera position
	normLook = XMVector3NormalizeEst(normLook); //Normalize the vector to make it a direction vector

	XMVECTOR translate = XMVectorMultiply(s, normLook); //multiply the speed vector with the direction vector
	XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate); //use the translate vector to create a translation matrix



	XMStoreFloat3(&viewPosition, XMVector3TransformCoord(p, translateMatrix)); //Multiply the camera position with the translation matrix and store it
	
	l = XMVector3TransformCoord(l, translateMatrix); //Translate the look at point to follow the player position
	
	
	XMStoreFloat3(&viewLookAt, l); //store the "look at"


	updateView(); //Update view matrix
}

void Camera::strafe(float d)
{
	
	//Strafing uses the same theory as walking. But we make use of a vector that keeps control of where our right side is.

	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&viewRightDirection);
	XMVECTOR p = XMLoadFloat3(&viewPosition);
	
	DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
	
	XMVECTOR translate = XMVectorMultiply(s, r); //create a translate vector of the speed multiplied with the "right hand" vector 
	
	XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate); //Create a translation matrix from the vector

	XMStoreFloat3(&viewPosition, XMVector3TransformCoord(p, translateMatrix)); //Translate the viewposition
	
	l = XMVector3TransformCoord(l, translateMatrix); //translate the Look at to follow the camera
	
	XMStoreFloat3(&viewLookAt, l); //store the new "Look at"


	updateView();



}
void Camera::updateView()
{


	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
		(
			DirectX::XMLoadFloat3(&viewPosition),
			DirectX::XMLoadFloat3(&viewLookAt),
			DirectX::XMLoadFloat3(&viewUpDirection)
			);



	//converting the view matrix into an XMFLOAT4X4, for simpler use
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	DirectX::XMFLOAT4X4 returnView;
	DirectX::XMStoreFloat4x4(&this->view, viewMatrix);



}
