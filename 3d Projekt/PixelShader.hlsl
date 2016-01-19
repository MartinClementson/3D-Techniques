cbuffer lightBuffer
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};




struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float4 wPos: WORLDPOS;
	float3 camPos : CAMERAPOS;

};


float4 PS_main(PS_IN input) : SV_TARGET
{

 //The light ray from the vert position to the light
//normalized to be used as a direction vector
float3 vRay = normalize((float3)(lightPosition - input.wPos));

//create a ray from the vert pos to the camera.
float3 v = normalize(input.camPos - input.wPos.xyz);

//Reflect is used in the specular shading
float3 r = reflect(-vRay, normalize(input.normal));

//Calculate how much of the pixel is to be lit
float fDot = max(0.0f, dot(normalize(vRay), normalize(input.normal)));

float3 color = lightColor.xyz;

float3 lightColor = mul(color, intensity);

float shinyPower = 20.0f;

float3 specularLight = { lightColor.xyz * pow(max(dot(r,v),0.0),shinyPower) };

float3 diffuse = input.color * fDot;

float3 ambient = { 0.1f, 0.1f, 0.1f };

ambient = (color *ambient);
diffuse = diffuse * lightColor;

float4 col = { (ambient + diffuse + specularLight),1.0 };

return col;
}