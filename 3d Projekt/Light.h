#pragma once
#include "Linker.h"

class Light
{
private:
	DirectX::XMFLOAT4 lightPosition;
	DirectX::XMFLOAT4 lightColor;
	float intensity;
public:
	Light();
	Light(DirectX::XMFLOAT4 newLightPosition, DirectX::XMFLOAT4 newLightColor, float newIntensity);
	virtual ~Light();

	//Setting
	void setLightPosition(DirectX::XMFLOAT4 newLightPosition)
	{ this->lightPosition = newLightPosition; }

	void setLightColor(DirectX::XMFLOAT4 newLightColor)
	{ this->lightPosition = newLightColor; }

	void setIntensity(float newIntensity)
	{ this->intensity = newIntensity; }
	//Getting
	DirectX::XMFLOAT4 getLightPosition() { return this->lightPosition; }
	DirectX::XMFLOAT4 getLightColor() { return this->lightColor; }
	float getIntensity() { return this->intensity; }
};

