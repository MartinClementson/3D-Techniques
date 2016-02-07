cbuffer lightBuffer
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};

SamplerState SampleType;

//modifies how the pixels are written to the polygon face when shaded
Texture2D shaderTexture;


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

	float3 textureSample = shaderTexture.Sample(SampleType, input.Texture).xyz;
	//float3 diffuse = textureSample * fDot;

	
	//finalCol = textureSample* finalCol; // texture * (diffuse + ambient)
	//finalCol = finalCol + specularLight; // + specular

										 //float4 col ={ (ambient + diffuse + specularLight),1.0 }; //old Calculation

	float4 col = {textureSample,1.0 };
	return col;
}