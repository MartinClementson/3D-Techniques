#include "Light.h"


using namespace DirectX;
Light::Light()
{
	lightColor = XMFLOAT4(1, 1, 1, 1);
	lightPosition = XMFLOAT4(0, 0, -2, 1);
	intensity = 1.0f;
}

Light::Light(DirectX::XMFLOAT4 newLightPosition, DirectX::XMFLOAT4 newLightColor, float newIntensity)
{
	this->lightPosition = newLightPosition;
	this->lightColor = newLightColor;
	this->intensity = newIntensity;
}

Light::~Light()
{
}
