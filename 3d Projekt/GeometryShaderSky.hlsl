cbuffer worldConstantBuffer : register(b0)
{
	matrix world;
	matrix normalWorld;
};


cbuffer cameraConstantBuffer  : register(b1)
{

	matrix projection;
	matrix view;
	float3 camLook;
	float3 camPos;
	// normalworld?
};

struct GSinput
{

	float4 pos : SV_POSITION;
	float2 Texture: TEXCOORD0;

};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 Texture: TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos : WORLDPOS;
	float3 camPos: CAMERAPOS;
};

[maxvertexcount(3)]
void GS_main(
	triangle GSinput input[3],
	inout TriangleStream< GSOutput > output)

{
	float3 faceEdgeA = input[1].pos - input[0].pos;
	float3 faceEdgeB = input[2].pos - input[0].pos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));




	//combining the matrices for simpler use, also more efficient
	matrix combinedMatrix = mul(world, mul(view, projection));

	
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i].pos;
		//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
		element.pos = mul(element.pos, combinedMatrix).xyww;
		element.wPos = input[i].pos;
		element.camPos = camPos;
		element.Texture = input[i].Texture;


		element.normal = mul(faceNormal, normalWorld);
		output.Append(element);
	}
	
}