cbuffer worldConstantBuffer : register(b0) //temporary, going to be moved into the GS
{
	matrix world;
	
	// normalworld?
};


cbuffer cameraConstantBuffer  : register(b1)//temporary, going to be moved into the GS
{
	
	matrix projection;
	matrix view;
	// normalworld?
};
//creating structs to simplify the handling
struct VS_IN
{
	float3 pos : POSITION;
	float3 color : COLOR;
	//put normals here later
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	//put normals here later
};



VS_OUT VS_main( VS_IN input )
{
	VS_OUT output; //perhaps its supposed to say VS_OUT output = (VS_OUT)0;

	//Multiplying the matrices for simpler use
	matrix combinedMatrix = mul(world, mul(projection, view)); //This order works, but it should´be view before projection, i think

	//setting the attributes
	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(output.pos, combinedMatrix);

	output.color = input.color;

	return output;
}