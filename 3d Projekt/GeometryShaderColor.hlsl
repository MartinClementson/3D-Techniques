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
	float3 color : COLOR;

};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
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
	float dt = -0.5;
	matrix combinedMatrix = mul(world, mul(projection, view));

	//Calculating backface culling
	//for (uint j = 0; j < 3; j++)
	//{
	//	float3 faceNormal2 = normalize(mul(faceNormal, normalWorld));
	//	float3 viewDir = normalize(camPos - normalize(mul(input[j].pos.xyz, world)));
	//	//viewDir = normalize(mul(viewDir, view));
	//	dt = max(dot(-viewDir, faceNormal2), 0.0f);
	//	if (dt > 0)
	//		break;
	//}

	float3 faceNormal2 = normalize(mul(faceNormal, normalWorld));
	float3 viewDir = normalize(mul(input[0].pos.xyz, world));
	viewDir = normalize(viewDir - camPos);

	dt = dot(viewDir, faceNormal2);




	if (dt > -0.25)
	{
		//combining the matrices for simpler use, also more efficient

		//matrix combinedMatrix = mul(world, view);
		//combinedMatrix = mul(combinedMatrix, projection);

		for (uint i = 0; i < 3; i++)
		{
			GSOutput element;
			element.pos = input[i].pos;
			element.pos = mul(element.pos, combinedMatrix);
			element.wPos = mul(element.pos, world);
			element.camPos = camPos;
			element.color = input[i].color;


			element.normal = normalize(mul(faceNormal, normalWorld));
			output.Append(element);
		}
	}
}