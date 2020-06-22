//Simple depth geometry shader; similar to the other geometry shader
//apply the explosion effect to the depth/shadows instead
//no normal or texture calculation

//Declare constant buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer CameraBuffer : register(b1)
{
    float time;
    float3 padding;
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
    float4 depthPosition : TEXCOORD0;
};

[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
    float4 newPosition;

   //Calculate the new normal vector by using the cross product and getting the perpendicular vector; the normal vector
    float3 a = input[0].position - input[1].position;
    float3 b = input[2].position - input[1].position;
    float3 N = normalize(cross(a, b));


    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[0].position + float4(N * ((sin(time) + 1.0)), 0.0f);

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Save the position into a secondary variable
    output.depthPosition = output.position;

    //Save the vertex and move onto the next one in the triangle
    triStream.Append(output);


    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[1].position + float4(N * ((sin(time) + 1.0)), 0.0f);

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Save the position into a secondary variable
    output.depthPosition = output.position;

    //Save the vertex and move onto the last on
    triStream.Append(output);


    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[2].position + float4(N * ((sin(time) + 1.0)), 0.0f);

    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Save the position into a secondary variable
    output.depthPosition = output.position;

    //Save the last vertex
    triStream.Append(output);

    //Move onto the next triangle
    triStream.RestartStrip();
}