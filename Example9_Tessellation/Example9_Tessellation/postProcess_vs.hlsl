//Vertex shader for post processing; calculates the position of the ortho mesh based on the world, view and projection matrix
//Pass through the texture coordinates

//Declare constant buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

OutputType main(InputType input)
{
    OutputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Pass the throught he texture coordinates
    output.tex = input.tex;

    return output;
}