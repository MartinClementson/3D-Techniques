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
	viewRightDirection = viewPosition + XMFLOAT3{ 1.0f, 0.0f, 0 };
	viewUpDirection = /*viewPosition + */XMFLOAT3{ 0, 1.0f, 0 };
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
	//Yaw is the rotation from left to right 
	// To get the axis in which we are to rotate. we will take the cross product of the direction vector and the right hand vector


	//DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d); //speed
	DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
	DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position
	DirectX::XMVECTOR r = XMLoadFloat3(&viewRightDirection);

	XMVECTOR normLook = XMVectorSubtract(l, p); //create a vector from the "look at" point ant the camera position
	//normLook = XMVector3NormalizeEst(normLook); //Normalize the vector to make it a direction vector
	XMVECTOR normRight = XMVectorSubtract(r, p); //create a vector from the "Right direction" point and the camera position
	
	XMVECTOR axis = XMVector3Cross(normLook, normRight);

	axis = XMVectorAdd(p, axis);

	if (XMVector3Equal(axis, XMVectorZero()))
		return;
	//axis = XMVector3Normalize(axis);
	//XMMATRIX yawMatrix = XMMatrixRotationRollPitchYaw(0.0f,XMConvertToRadians(angle),0.0f);
	//XMMATRIX yawMatrix = XMMatrixRotationAxis(axis,XMConvertToRadians(angle));
	XMMATRIX yawMatrix = XMMatrixRotationY(XMConvertToRadians(angle));

	XMVECTOR right = XMVector3Transform(normRight, yawMatrix);
	//right = XMVector3Normalize(right);

	right = XMVectorAdd(p ,right);
	XMStoreFloat3(&viewRightDirection,right);

	//XMStoreFloat3(&viewUpDirection, XMVector3TransformNormal(XMLoadFloat3(&viewUpDirection), yawMatrix));
	l = XMVector3Transform(normLook, yawMatrix);
	
	l = XMVectorAdd(p, l);
	XMStoreFloat3(&viewLookAt, l);
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

	XMStoreFloat3(&viewLookAt, XMVector3Transform(XMLoadFloat3(&viewLookAt), pitchMatrix));


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
	DirectX::XMVECTOR r = XMLoadFloat3(&viewRightDirection);

	

											
	XMVECTOR normLook = XMVectorSubtract(l, p); //create a vector from the "look at" point ant the camera position
	normLook = XMVector3NormalizeEst(normLook); //Normalize the vector to make it a direction vector

	XMVECTOR translate = XMVectorMultiply(s, normLook); //multiply the speed vector with the direction vector
	XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate); //use the translate vector to create a translation matrix



	XMStoreFloat3(&viewPosition, XMVector3TransformCoord(p, translateMatrix)); //Multiply the camera position with the translation matrix and store it
	
	l = XMVector3TransformCoord(l, translateMatrix); //Translate the look at point to follow the player position
	
	r = XMVector3TransformCoord(r, translateMatrix);
	XMStoreFloat3(&viewRightDirection, r);
	
	XMStoreFloat3(&viewLookAt, l); //store the "look at"


	updateView(); //Update view matrix
}

void Camera::strafe(float d)
{

	//Strafing uses the same theory as walking. But we make use of a vector that keeps control of where our right side is.

	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&viewRightDirection);
	XMVECTOR p = XMLoadFloat3(&viewPosition);
	XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at

	XMVECTOR normRight = XMVectorSubtract(r, p); //create a vector from the "Right direction" point and the camera position
	normRight = XMVector3NormalizeEst(normRight); //Normalize the vector to make it a direction vector


	XMVECTOR translate = XMVectorMultiply(s, normRight); //create a translate vector of the speed multiplied with the "right hand" vector 
	XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate); //Create a translation matrix from the vector


	p = XMVector3TransformCoord(p, translateMatrix);
	XMStoreFloat3(&viewPosition, p); //Translate the viewposition



	r = XMVector3TransformCoord(r, translateMatrix); //Translate the Right direction 
	XMStoreFloat3(&viewRightDirection, r);

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

	DirectX::XMStoreFloat4x4(&this->view, viewMatrix);



}
