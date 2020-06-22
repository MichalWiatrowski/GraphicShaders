//Simple pass through vertex shader; passes through position, texture  coordinates and normals to the geometry shader
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

OutputType main(InputType input)
{
    OutputType output;

    // Pass the input data to the geometry shader stage
    output = input;
   
    return output;
}