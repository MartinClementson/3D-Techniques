cbuffer lightBuffer
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};

SamplerState SampleType;

//modifies how the pixels are written to the polygon face when shaded
Texture2D shaderTexture : register(t0);
textureCUBE skyBoxTexture : register(t1);
Texture2D renderTexture : register(t2);
textureCUBE dynamicTexture : register(t3);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 Texture : TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos: WORLDPOS;
	float3 camPos : CAMERAPOS;

};


float4 PS_main(PS_IN input) : SV_TARGET
{

	/*
		Using specular light on this, makes the edges of the quads visible,
		until we fix the normals. Don't use the specular on this
	
	*/



 
   
float3 textureSample = dynamicTexture.Sample(SampleType, input.Texture).xyz;



    float4 col = { textureSample, 1.0 };
    return col;
}