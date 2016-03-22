//group will be [32,30,1], in the end we will get 800X600
//which is the Resolution

struct outputStruct
{
	float4 color;
};

Texture2D inputTexture : register(t0);
//RWStructuredBuffer<outputStruct> output : register(u0);
RWTexture2D<float4> output;
[numthreads(25, 20, 1)]
void main( uint3 DTid : SV_DispatchThreadID)
{
	
	int3 texturelocation = int3(0, 0, 0);
	texturelocation.x = DTid.x;
	texturelocation.y = DTid.y;
	float4 color = { 1.0, 0.0, 0.0, 1.0 };
	color = inputTexture.Load(texturelocation);
	//color.x += 0.3;
	output[DTid.xy] = color ;


}