


struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;

};


float4 PS_main(PS_IN input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}