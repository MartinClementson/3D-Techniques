cbuffer lightBuffer
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};

SamplerState SampleType;

//modifies how the pixels are written to the polygon face when shaded
textureCUBE shaderTexture : register(t1); //the skybox is a texture cube,
Texture2D renderTexture : register(t2);
///It consists of 6 separate textures


struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos: WORLDPOS;
	float3 camPos : CAMERAPOS;

};


float4 PS_main(PS_IN input) : SV_TARGET
{

	//To sample a cube map. 3d coorinates are used, not UV.


	//Use the verts worldPos when sampling

	float3 textureSample = shaderTexture.Sample(SampleType, input.wPos).xyz;
	

	float4 col = { textureSample,1.0 };
	return col;
}