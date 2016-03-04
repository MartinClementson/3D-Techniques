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
Texture2D normalTexture : register(t2);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos: WORLDPOS;
	float3 camPos : CAMERAPOS;
	float3 Tangent:TANGENT;
	
};

float3 normalToWorldSpace(float3 normalMapSample, float3 normal, float3 tangent)
{
	// here we build the tbn basis. to transform the sampled normal from tangent space to the world space
	//then we return the normal in world

	//Convert from [0,1] to [-1,1]
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	//Build basis
	float3 N = normal;
	float3 T = normalize(tangent - dot(tangent, N)* N); //Read page 582
	float3 B = cross(N, T); //Bitangent

	float3x3 TBN = float3x3(T, B, N);

	//Transform from tangent space to world space

	float3 bumpedNormal = mul(normalT, TBN);

	return bumpedNormal;

}

float4 PS_main(PS_IN input) : SV_TARGET
{
//sampling the normal
float3 normalSample = normalTexture.Sample(SampleType, input.Texture).rgb;

float3 bumpedNormal = normalToWorldSpace(normalSample, input.normal, input.Tangent);


 //The light ray from the vert position to the light
//normalized to be used as a direction vector
float3 vRay = normalize((float3)(lightPosition - input.wPos));

//create a ray from the vert pos to the camera.
float3 v = normalize(input.camPos - input.wPos.xyz);

//Reflect is used in the specular shading
float3 r = reflect(-vRay, normalize(bumpedNormal));

//Calculate how much of the pixel is to be lit
float fDot = max(0.0f, dot(normalize(vRay), normalize(bumpedNormal)));

float3 color = lightColor.xyz;

float3 lightColor = mul(color, intensity);

float shinyPower = 20.0f;

float3 specularLight = { lightColor.xyz * pow(max(dot(r,v),0.0),shinyPower) };

float3 textureSample = shaderTexture.Sample(SampleType, input.Texture).xyz;
//float3 diffuse = textureSample * fDot;

float3 ambient = { 0.5f, 0.5f, 0.5f };



///////
//Computing the final color with a "late add" of the specular light.
//with late add the computation is modular, allowing for multiple lights
//
//late add is : texturecolor * (diffuse + ambient) + specular
//
// 3d game programming book. p.330
///////


float3 diffuse = lightColor * fDot;


float3 finalCol = (diffuse + ambient);
finalCol = textureSample* finalCol; // texture * (diffuse + ambient)
finalCol = finalCol + specularLight; // + specular

//float4 col ={ (ambient + diffuse + specularLight),1.0 }; //old Calculation

//Calculate enviroment reflections
//float3 incident = input.wPos - input.camPos;
//float3 ref = reflect(incident, normalize(input.normal));
//float4 reflectionColor = skyBoxTexture.Sample(SampleType, ref);
//finalCol += reflectionColor.xyz *0.5;
//
//finalCol.x = min(finalCol.x, 1.0f);
//finalCol.y = min(finalCol.y, 1.0f);
//finalCol.z = min(finalCol.z, 1.0f);


float4 col = { finalCol,1.0 };
return col;
}