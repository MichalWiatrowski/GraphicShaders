// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

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

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
    float4 vertexPosition;
    float2 textureCoor;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).

    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z *
    patch[2].position;


    textureCoor = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position;

    //Determine the new texture coordinates
    //float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.x);
  //  float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.x);

    //textureCoor = lerp(t1, t2, uvwCoord.y);


    output.position = vertexPosition;
    output.tex = textureCoor;
    output.normal = 1;

    //float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.x);
    //float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.x);

   // vertexPosition = lerp(v1, v2, uvwCoord.y);
    
		    
    // Calculate the position of the new vertex against the world, view, and projection matrices.
   // output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
   // output.position = mul(output.position, viewMatrix);
   // output.position = mul(output.position, projectionMatrix);

    // Send the input color into the pixel shader.
    //output.colour = patch[0].colour;

    return output;
}

