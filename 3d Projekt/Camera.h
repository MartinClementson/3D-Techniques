#pragma once
#include "Linker.h"
using std::max;
using std::min;
using namespace DirectX;
class Camera
{
private:
	//projection Matrix
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 view;
	
	//view Matrix parameters
	DirectX::XMFLOAT3 viewUpDirection;
	DirectX::XMFLOAT3 viewRightDirection;
	DirectX::XMFLOAT3 viewPosition;
	DirectX::XMFLOAT3 viewLookAt;
	

public:
	void walk(float d);
	void strafe(float d);
	Camera();
	virtual ~Camera();

	void rotateYaw(float angle); // yaw == around y axis
	void rotatePitch(float angle);
	
	void updateView();
	
	
	
	
	
	
	//Setting
	void setViewPosition(DirectX::XMFLOAT3 nViewPos);  //Maybe a different parameter?
	void setViewLookAt(DirectX::XMFLOAT3 nViewLookAt);
	void setViewUpDirection(XMFLOAT3 nViewUpDirection);
	void setCameraLens(float angle, float ratio, float nearPlane, float farPlane);
	//Getting
	DirectX::XMFLOAT3 getCamLookAt();
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getProjection();
	DirectX::XMFLOAT3 getCamPos();



	
	

};

