//Simple vertex shader that passes through all the data into the geometry shader
struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

OutputType main(InputType input)
{
    OutputType output;

	//Set the output equal to the input
    output = input;

    // Pass all of the inputs into the geometry shader
    return output;
}
