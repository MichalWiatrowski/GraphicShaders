//Simple pixel shader that samples the texture and returns it 
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	// Sample the texture.
    float4 textureColour = texture0.Sample(sampler0, input.tex);
   
    //Return texture for final pixel colour
    return textureColour;
}

