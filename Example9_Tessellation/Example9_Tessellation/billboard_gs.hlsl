//Billboard geometry shader
//The shader will take the point and create four vertices around the points depending on the size passsed through forming a quad.
//Do simple texture coordinates and calculate normals based on the view matrix

//Declare constant buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer SizeBuffer : register(b1)
{
    float billSize;
    float3 padding;
}

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
    float3 normal : NORMAL;
};

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
	
    //Code written by Michal Wiatrowski, adapted from the following references
    //http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
    //https://gamedev.stackexchange.com/questions/113147/rotate-billboard-towards-camera
    
	//calculate the right and up camera vectors in world space
    float3 CameraRight = float3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    float3 CameraUp = float3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    int size = billSize; // the offset for the vertices from the point

    //calculate the offset vertex
    float4 vposition = float4(input[0].position.xyz + CameraRight * size + CameraUp * -size, 1.0);
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(vposition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = float2(1, 1); //top right

    // Calculate the normal vector against the view matrix only instead of world because the mesh is being rotated to always look at the camera, then normalise it
    output.normal = mul(input[0].normal, (float3x3) viewMatrix);
    output.normal = normalize(output.normal);

    triStream.Append(output); //append vertex

    vposition = float4(input[0].position.xyz + CameraRight * size + CameraUp * size, 1.0);
    output.position = mul(vposition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = float2(1, 0); //bottom right
    output.normal = mul(input[0].normal, (float3x3) viewMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

    vposition = float4(input[0].position.xyz + CameraRight * -size + CameraUp * -size, 1.0);
    output.position = mul(vposition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = float2(0, 1); //top left
    output.normal = mul(input[0].normal, (float3x3) viewMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

    vposition = float4(input[0].position.xyz + CameraRight * -size + CameraUp * size, 1.0);
    output.position = mul(vposition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = float2(0, 0); //bottom left
    output.normal = mul(input[0].normal, (float3x3) viewMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

    triStream.RestartStrip();
}