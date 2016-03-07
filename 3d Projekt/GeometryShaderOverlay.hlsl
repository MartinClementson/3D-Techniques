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
};

[maxvertexcount(6)]
void GS_main(
	point GSinput input[1],
	inout TriangleStream< GSOutput > output)

{

    float4 hej[6];
    hej[0] = float4(-1.0f, 1.0f, 0.0f, 1.0f);
    hej[1] = float4(1.0f, -1.0f, 0.0f, 1.0f);
    hej[2] = float4(-1.0f, -1.0f, 0.0f, 1.0f);

    hej[3] = float4(1.0f, -1.0f, 0.0f, 1.0f);
    hej[4] = float4(-1.0f, 1.0f, 0.0f, 1.0f);
    hej[5] = float4(1.0f, 1.0f, 0.0f, 1.0f);



	//float3 faceEdgeA = hej[1] - hej[0];
	//float3 faceEdgeB = hej[2] - hej[0];
	//float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	


	

	//////////////////////////////////////////////////
	///////////Back-face culling test/////////////////

	//multiply the face normal into world space
	//float3 faceNormal2 = normalize(mul(faceNormal, normalWorld));

	////multiply one of the vertices into world space
	//float3 viewDir = mul(input[0].pos, world);

	////Create a vector from that vertex to the cameras position
	//viewDir = normalize(viewDir - camPos);

	////Take the dot product of the flipped vert<-cam vector and the normal
	//float dt = dot(-viewDir, faceNormal2);

	//if the dot product is less than 0, The angle is more than 90 degrees, which means we can't see it
	///////////////////////////////////////////////////////////

	//Note: To turn it off, Comment the IF statement below. 
	//To show that it works : Create a vector from 0,0,-1  to viewDir,
    float dt = 1.0;


	if (dt > 0)
	{
		//combining the matrices for simpler use, also more efficient

		matrix combinedMatrix = mul(world, mul(view, projection));
		//matrix combinedMatrix = mul(world, view);
		//combinedMatrix = mul(combinedMatrix, projection);
      

		for (uint i = 0; i < 3; i++)
		{
			GSOutput element;
            element.pos = hej[i];
			
			element.color = input[0].color;


			
			output.Append(element);
		}
        output.RestartStrip();
        for (uint i = 3; i < 6; i++)
        {
            GSOutput element;
            element.pos = hej[i];
			
            element.color = input[0].color;


            output.Append(element);
        }




	}
}