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
	viewUpDirection = XMFLOAT3{ 0, 1.0f, 0 };
	m_frustum = new Frustum();

	
	

	//creating the projection matrix
	DirectX::XMMATRIX tempProjection;
	tempProjection = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI*0.45f), (WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, SCREEN_DEPTH);
	

	tempProjection = DirectX::XMMatrixTranspose(tempProjection);
	DirectX::XMStoreFloat4x4(&projection, tempProjection);
	updateView();
	

}
Camera::Camera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 lookat)
{
	this->viewPosition = pos;
	this->viewLookAt = lookat;
	viewRightDirection = viewPosition + XMFLOAT3{ 1.0f, 0.0f, 0 };
	viewUpDirection = XMFLOAT3{ 1.0f, 0.0f, 0 };
	m_frustum = new Frustum();

	//creating the projection matrix
	DirectX::XMMATRIX tempProjection;
	tempProjection = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI*0.45f), (WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, SCREEN_DEPTH);


	tempProjection = DirectX::XMMatrixTranspose(tempProjection);
	DirectX::XMStoreFloat4x4(&projection, tempProjection);
	updateView();


}

Camera::~Camera()
{

	if (m_frustum != nullptr)
		delete m_frustum;
}

void Camera::setViewPosition(DirectX::XMFLOAT3 nViewPos)
{
	this->viewPosition = nViewPos;
	
	

}

void Camera::setViewLookAt(DirectX::XMFLOAT3 nViewLookAt)
{
	
	this->viewLookAt = nViewLookAt;
	
}

void Camera::setViewUpDirection(XMFLOAT3 nViewUpDirection)
{
	this->viewUpDirection = nViewUpDirection;
}

void Camera::setCameraLens(float angle, float ratio, float nearPlane, float farPlane)
{
	this->farPlaneDistance = farPlane;
	DirectX::XMMATRIX tempProjection;
	
	tempProjection = DirectX::XMMatrixPerspectiveFovLH(angle, ratio, nearPlane, farPlane);
	tempProjection = DirectX::XMMatrixTranspose(tempProjection);
	DirectX::XMStoreFloat4x4(&projection, tempProjection);
}

void Camera::rotateYaw(float angle)
{
	DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
	DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position
	DirectX::XMVECTOR r = XMLoadFloat3(&viewRightDirection); // right direction

	XMVECTOR normLook = XMVectorSubtract(l, p); //create a vector from the "look at" point ant the camera position

	XMVECTOR normRight = XMVectorSubtract(r, p); //create a vector from the "Right direction" point and the camera position
	
	//Create the rotation matrix, Up will always be +Y, so we will always rotate around it
	XMMATRIX yawMatrix = XMMatrixRotationY(XMConvertToRadians(angle));


	///////
	//Rotate the Right Direction vector and move it to the player position
	XMVECTOR right = XMVector3Transform(normRight, yawMatrix); 
	right = XMVectorAdd(p ,right);
	XMStoreFloat3(&viewRightDirection,right);
	///////


	//////
	//Rotate the "lookAt" direction vector and move it to the player position
	l = XMVector3Transform(normLook, yawMatrix);
	l = XMVectorAdd(p, l);
	XMStoreFloat3(&viewLookAt, l);
	/////
	

}


void Camera::rotatePitch(float angle)
{
	DirectX::XMVECTOR l = XMLoadFloat3(&viewLookAt); // look at
	DirectX::XMVECTOR p = XMLoadFloat3(&viewPosition); // position
	DirectX::XMVECTOR r = XMLoadFloat3(&viewRightDirection); // right direction
	DirectX::XMVECTOR u = XMLoadFloat3(&viewUpDirection); // right direction



	XMVECTOR normLook = XMVectorSubtract(l, p); //create a vector from the "look at" point ant the camera position

	XMVECTOR normRight = XMVectorSubtract(r, p); //create a vector from the "Right direction" point and the camera position

	XMMATRIX pitchMatrix = XMMatrixRotationAxis(normRight, XMConvertToRadians(angle));

	//////
	//Rotate the "lookAt" direction vector and move it to the player position
	l = XMVector3Transform(normLook, pitchMatrix);

	//Here we do a test to make sure we don't go to far up or down
	//Note: a optimization is needed
	
	//Get the angle between the look at vector and the up and negative up vector
	XMVECTOR angleDiff = XMVector3AngleBetweenVectors(l, u);
	XMVECTOR angleDiffNeg = XMVector3AngleBetweenVectors(l, -u);

	//test their Y values (The up vector will always be 0,1,0. atleast for now. Changes might be done in the future )
	if (angleDiff.m128_f32[1] <= 0.2) 
		return;
	if (angleDiffNeg.m128_f32[1] <= 0.2)
		return;

	l = XMVectorAdd(p, l);
	XMStoreFloat3(&viewLookAt,l );
	/////

	

}

DirectX::XMFLOAT4X4 Camera::getView()
{
	
	updateView();
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
DirectX::XMFLOAT3 Camera::getCamLookAt()
{
	return viewLookAt;
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
	
	





}
void Camera::updateView()
{


	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
		(
			DirectX::XMLoadFloat3(&viewPosition),
			DirectX::XMLoadFloat3(&viewLookAt),
			DirectX::XMLoadFloat3(&viewUpDirection)
			);


	DirectX::XMFLOAT4X4 viewConvert;
	DirectX::XMStoreFloat4x4(&viewConvert, viewMatrix);
	XMMATRIX tempProj = XMLoadFloat4x4(&this->projection);
	tempProj = XMMatrixTranspose(tempProj);
	XMFLOAT4X4 projConvert;
	XMStoreFloat4x4(&projConvert, tempProj);
	DirectX::XMStoreFloat4x4(&viewConvert, viewMatrix);

	m_frustum->ConstructFrustum(this->farPlaneDistance, projConvert, viewConvert);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	//converting the view matrix into an XMFLOAT4X4, for simpler use
	DirectX::XMStoreFloat4x4(&this->view, viewMatrix);



}

void Camera::teleportToTerrain()
{

	this->viewPosition = XMFLOAT3(60, 30, 40);
	


}
