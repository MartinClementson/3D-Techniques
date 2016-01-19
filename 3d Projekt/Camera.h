#pragma once
#include "Linker.h"
class Camera
{
private:
	//projection Matrix
	DirectX::XMFLOAT4X4 projection;
	float yaw; //rotation around y axis
	float pitch; //Rotation around x axis

	//view Matrix parameters
	DirectX::XMVECTOR viewUpDirection;
	DirectX::XMVECTOR viewRightDirection;
	DirectX::XMVECTOR viewPosition;
	DirectX::XMVECTOR viewLookAt;

public:
	Camera();
	virtual ~Camera();

	//Setting
	void setViewPosition(DirectX::XMFLOAT3& nViewPos);  //Maybe a different parameter?
	void setViewLookAt(DirectX::XMFLOAT3& nViewLookAt);
	//set upDir?
	void rotateYaw(float angle); // yaw == around y axis
	void rotatePitch(float angle);
	//Getting
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getProjection();
	DirectX::XMFLOAT3 getCamPos();
	

};

