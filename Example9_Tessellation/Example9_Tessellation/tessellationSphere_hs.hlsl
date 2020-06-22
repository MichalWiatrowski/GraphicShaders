// Tessellation Hull Shader
// Prepares control points for tessellation
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct OutputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

cbuffer TesselationValues : register(b1)
{
    float4 edges4;
    float2 insideTess;
    float2 padding;
    float3 cameraPos;
    float padding2;
    float2 minMax;
    float2 padding3;
};
ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
    //code adapted from unreal blueprint https://answers.unrealengine.com/questions/11704/tessellation-camera-distance-lod.html

   // float3 distance = abs(inputPatch[patchId].position.xyz - cameraPos.xyz); //calculate the distance from the camera and the object being tessellated

  //  float tessScale =  64 / length(distance); //divide the max tess value by the distance

   // float tessFactor = clamp(tessScale, 0, 64); //final tessellation calculation
    float tessFactor = minMax.x;

    // Set the tessellation factors for the four edges of the quad.
    output.edges[0] = tessFactor; // tessellationFactor;
    output.edges[1] = tessFactor; //tessellationFactor;
    output.edges[2] = tessFactor; // tessellationFactor;
    

    // Set the tessellation factor for tessallating the horizontal and vertical inside of the quad.
    output.inside = tessFactor; // tessellationFactor;
    

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
  //  output.position = patch[pointId].position;
    output = patch[pointId];

    // Set the input colour as the output colour.
   // output.colour = patch[pointId].colour;

    return output;
}