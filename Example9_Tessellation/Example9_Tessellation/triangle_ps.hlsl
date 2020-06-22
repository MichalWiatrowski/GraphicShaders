Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

cbuffer LightBuffer : register(b0)
{
    float4 diffuseColour;
    float4 ambientColour;
    float3 lightDirection;
    float padding;
};
// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse, float4 ambient)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = ambient;
    colour += saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float alpha = 0;
    float4 textureColour = texture0.Sample(Sampler0, input.tex);

        
        float4 lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour, ambientColour);


    return float4(1.0f, 0.0f, 0.0f, 1.0f) * lightColour ;


}