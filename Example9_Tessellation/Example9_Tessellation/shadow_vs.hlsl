//Simple vertex shader calculating positons against the matrices and calculates the view positon of the lights

//Declare constant buffers
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[3];
	matrix lightProjectionMatrix[3];
};
cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
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
	float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
    float4 lightViewPos3 : TEXCOORD3;
    float3 viewVector : TEXCOORD4;
    float3 worldPosition : TEXCOORD5;

};

OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPos = mul(input.position, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix[0]);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix[0]);

    output.lightViewPos2 = mul(input.position, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrix[1]);
    output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrix[1]);

    output.lightViewPos3 = mul(input.position, worldMatrix);
    output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix[2]);
    output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix[2]);

    //Calculate the world position of the new vertex
    float4 worldPosition = mul(input.position, worldMatrix);

    //Calculate the view vector between the camera position and the world position of the vertex
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);
    
    // pass through the world position
    output.worldPosition = worldPosition.xyz;
   
    //pass through the texture coordinates
    output.tex = input.tex;

    //Calculate the normal vector against the world matrix and then normalize
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}