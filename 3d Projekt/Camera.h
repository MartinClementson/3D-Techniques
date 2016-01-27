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



	void updateView();
	
	

};

