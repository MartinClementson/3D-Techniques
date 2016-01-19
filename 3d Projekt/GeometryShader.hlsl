cbuffer worldConstantBuffer : register(b0) 
{
	matrix world;

	// normalworld?
};


cbuffer cameraConstantBuffer  : register(b1)
{

	matrix projection;
	matrix view;
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
	matrix combinedMatrix = mul(world, mul(projection, view));

	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i].pos;
		element.pos = mul(element.pos, combinedMatrix);
		element.color = input[i].color;
		element.normal = faceNormal;
		output.Append(element);
	}
}