//creating structs to simplify the handling
struct VS_IN
{
	float3 pos : POSITION;
	float2 Texture: TEXCOORD0;
	float3 Normal: NORMALS;
	float3 Tangent:TANGENT;
	float3 BiTangent:BITANGENT;

	//normals?
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal: NORMALS;
	//put normals here later
};



VS_OUT VS_main( VS_IN input )
{
	VS_OUT output; //perhaps its supposed to say VS_OUT output = (VS_OUT)0;

	//setting the attributes
	output.pos = float4(input.pos, 1.0f);
	output.Texture = input.Texture;
	output.Normal = input.Normal;

	return output;
}