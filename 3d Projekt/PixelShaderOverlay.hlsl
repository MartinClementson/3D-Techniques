cbuffer lightBuffer : register(b0)
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};
cbuffer pixelShaderConstants : register(b1)
{
    bool miniMap;
    bool normalMap;
    bool distanceFog;

};

SamplerState SampleType;
Texture2D shaderTexture : register(t0);
textureCUBE skyBoxTexture : register(t1);
Texture2D miniMapTex : register(t4);
Texture2D ui : register(t5);
struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;

 

};

//THIS needs to be connected via a constant buffer to support different resolutions 
static float ScreenWidth = 800.0f;
static float ScreenHeight = 600.0f;
static float aspectRatio = 800.0f / 600.0f;

static float miniMapOffsetX = 10.0;
static float miniMapOffsetY = 10.0;


float4 PS_main(PS_IN input) : SV_TARGET
{

    float2 uv; 
    uv.x = (input.pos.x / ScreenWidth) ;
    uv.y = (input.pos.y / ScreenHeight);
    //Find the uv of the pixel, then scale it up with the scalar we use to minimize the minimap
    

    float4 col;

    //Render overlay
    col = ui.Sample(SampleType, uv);
    if (col.a == 1.0)
        return col;
        
   

    
    //render miniMap
    if (input.pos.x < (ScreenWidth / 4) )
    {
        if (input.pos.y < (ScreenHeight / 4))
        {
            
            uv.x *= 4;
            uv.y *= 4;

             //render a dot for the "player" position //if its the right pixel

            if (input.pos.x < ((ScreenWidth / 8) + 2) && input.pos.x > ((ScreenWidth / 8) - 2))
            {
                if (input.pos.y < float((ScreenHeight / 8) + 2) && input.pos.y > float((ScreenHeight / 8) - 2))
                {
        
            
                    col = float4(1.0, 0.0, 0.0, 0.3);
                }
                else
                    col = miniMapTex.Sample(SampleType, uv);
       
            }
            else
                col = miniMapTex.Sample(SampleType, uv);

            
        }
        else
            clip(-1);
    }
    else
        clip(-1);
        
//float4  //{ 1.0,0.0,0.0,1.0 };


return col;
}