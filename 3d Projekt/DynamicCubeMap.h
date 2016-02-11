#pragma once
#include "Linker.h"
#include "Camera.h"
#include "Engine.h"
class Engine;
/* Dynamic Cube mapping is well presented in Frank, D , lunas book
3D game programming with direct X
 p.559
*/
using namespace DirectX;


class DynamicCubeMap
{


private:
	ID3D11Device *gDevice;
	ID3D11DeviceContext* gDeviceContext;
	
	static const int CubeMapSize = 1024; //The resolution of the cube map (default = 256)
	//A dynamic cube map calculation is very expensive, therefore it is good to have low resolution

	//Since these viewports will be smaller than the back buffer, we need a separate depthBuffer, matching the size of this cube map
	ID3D11DepthStencilView* dynamicCubemapDSV;
	ID3D11Texture2D* depthTex;

	//It also needs it's own viewPort
	D3D11_VIEWPORT cubeMapViewPort;

	//The cube texture (consists of an array of 6 textures)
	ID3D11Texture2D* cubeTex;




	//We will have 6 render targets.
	ID3D11RenderTargetView* dynamicCubeMapRTV[6] = {nullptr};
	//With a camera each
	Camera* cubeMapCamera[6] = { nullptr };

	//and together they will create a cubemap 
	ID3D11ShaderResourceView* dynamicCubeMapResource = nullptr ;

	void BuildCubeFaceCamera(XMFLOAT3 position);

public:


	void Release();
	void Render(XMFLOAT3 position, Engine* engine);
	void Update();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	DynamicCubeMap();
	~DynamicCubeMap();
};

