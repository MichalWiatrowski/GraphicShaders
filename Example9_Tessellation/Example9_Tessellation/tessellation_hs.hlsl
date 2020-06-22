// Tessellation Hull Shader
// Prepares control points for tessellation

//Declare constant buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer TesselationValues : register(b1)
{
    float3 cameraPos;
    float planeID;
    float minMax;
    float3 padding;
    float hard_distance;
    float3 padding2;
   
};
struct InputType
{
  
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
  
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
    float tessFactor;

    //if distance based tesellation is not activated, set tessellation to the inputted value
    if (hard_distance == 0)
    {
        tessFactor = minMax;
      
    }
    //if distance based tessellation is activated
    else if (hard_distance == 1)
    {
        //code adapted from unreal blueprint https://answers.unrealengine.com/questions/11704/tessellation-camera-distance-lod.html
        //In addition; Frank D. Luna. Introduction to 3D GAME PROGRAMMING WITH DIRECTX 11, 2012

        //Find the center of the patch
        float3 currentVertex = 0.25f * (inputPatch[0].position.xyz + inputPatch[1].position.xyz + inputPatch[2].position.xyz + inputPatch[3].position.xyz);
        //Transform against the world matrix
        currentVertex = mul(float4(currentVertex, 1.0f), worldMatrix);
	
        float3 distance = abs(currentVertex - cameraPos.xyz); //calculate the distance from the camera and the patch being tessellated

        float tessScale = 64 / length(distance); //divide the max tess value by the distance
        tessFactor = clamp(tessScale, 0, 64); //final tessellation calculation
    }
   
    // Set the tessellation factors for the four edges of the quad.
    output.edges[0] = tessFactor; // tessellationFactor;
    output.edges[1] = tessFactor; //tessellationFactor;
    output.edges[2] = tessFactor; // tessellationFactor;
    output.edges[3] = tessFactor; //tessellationFactor

    // Set the tessellation factor for tessallating the horizontal and vertical inside of the quad.
    output.inside[0] = tessFactor; // tessellationFactor;
    output.inside[1] = tessFactor; //tessellationFactor

    return output;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position, texture coordinates and normals for this control point as the output.
    output.position = patch[pointId].position;
    output.tex = patch[pointId].tex;
    output.normal = patch[pointId].normal;

    return output;
}