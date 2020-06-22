// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

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

cbuffer SinWaveValues : register(b2)
{
    float3 time_speed_height;
    float padding3;
   
};
struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
   
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition;
    OutputType output;

    float2 textureCoor;
    
	//As demonstrated by Paul Robertson and as shown from the sources below, we need to interpolate the control points to get the position of the new vertex, we also do the same for the texture coordinates
    //http://codeflow.org/entries/2010/nov/07/opengl-4-tessellation/
    //https://gamedev.stackexchange.com/questions/87616/opengl-quad-tessellation-control-shader
    //https://github.com/przemyslawzaworski/Unity3D-CG-programming/blob/master/quad_tessellation.shader

    // Determine the position of the new vertex.
    float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.x);
    float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.x);

    vertexPosition = lerp(v1, v2, uvwCoord.y);
    
    //Determine the new texture coordinates
    float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.x);
    float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.x);

    textureCoor = lerp(t1, t2, uvwCoord.y);

    if (planeID == 1)
    {
      
        //Calculate the normals from the height map; source: https://stackoverflow.com/questions/5281261/generating-a-normal-map-from-a-height-map
        //The size will change depending on the size of the plane we are sampling the texture for, since we are using 0.01 in all of our calculations we need to make sure we calculate the normals in roughly the same range
        //The lower the size value the brighter the colours are, using 0.1 instead to darken it, 0.01 is probably the correct value though
        const float2 size = float2(0.1, 0.1);
        //Store the offset which will be used to access the current pixels neighbors
        const float3 off = float3(1, 0, -1);

        //Sample and calculate the neighbors pixels
        float s11 = texture0.SampleLevel(sampler0, vertexPosition.xz * 0.01, 0).x;
        float s01 = texture0.SampleLevel(sampler0, float2(vertexPosition.x + off.x, vertexPosition.z + off.y) * 0.01, 0).x;
        float s21 = texture0.SampleLevel(sampler0, float2(vertexPosition.x + off.z, vertexPosition.z + off.y) * 0.01, 0).x;
        float s10 = texture0.SampleLevel(sampler0, float2(vertexPosition.x + off.y, vertexPosition.z + off.x) * 0.01, 0).x;
        float s12 = texture0.SampleLevel(sampler0, float2(vertexPosition.x + off.y, vertexPosition.z + off.z) * 0.01, 0).x;

        //va - test as in source its s21-s01 instead but s21-s11 looks more accurate/better???
        //Calculate the vectors
       float3 va = normalize(float3(-size.x, s21 - s01, size.y));
       float3 vb = normalize(float3(size.y, s12 - s10, size.x));
        //Use the cross product of the 2 vectors to find out the normal vector, then normalize it
       float4 bump = normalize(float4(cross(va, vb), s01));

        //Set the output normals to the calculated normals
        output.normal = bump.xyz;

        //Displace the y position of the vertex depending on the colour of the sampled height map. Times by 16 in since that is how many bits are typically in a grayscale texture and height maps are sampled using grayscale
        vertexPosition.y = bump.a * 16;     
    }
   else if (planeID == 2)
    {
        //offset position based on sine wave
        vertexPosition.y = sin(vertexPosition.x + time_speed_height.x * time_speed_height.y) * time_speed_height.z;
        
        //modify normals
        output.normal.x = 1 - cos(vertexPosition.x + time_speed_height.x * time_speed_height.y) * time_speed_height.z;
        output.normal.y = abs(cos(vertexPosition.x + time_speed_height.x * time_speed_height.y)) * time_speed_height.z;
    }

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Set the output texture coordinates to the newly calculated coordinates
    output.tex = textureCoor;

    return output;
}

