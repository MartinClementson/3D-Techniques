#pragma once
#include "Linker.h"
class Camera
{
private:
	//projection Matrix
	DirectX::XMFLOAT4X4 projection;
	
	//view Matrix parameters
	DirectX::XMVECTOR viewUpDirection;
	DirectX::XMVECTOR viewPosition;
	DirectX::XMVECTOR viewLookAt;

public:
	Camera();
	virtual ~Camera();

	//Setting
	void setViewPosition(DirectX::XMVECTOR nViewPos);  //Maybe a different parameter?
	void setViewLookAt(DirectX::XMVECTOR nViewLookAt);
	//set upDir?

	//Getting
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getProjection();
	DirectX::XMFLOAT3 getCamPos();
	

};

