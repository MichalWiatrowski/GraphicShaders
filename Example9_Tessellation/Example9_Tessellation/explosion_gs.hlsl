//Geometry shader; will apply explosion effect by offsetting the vertex on the normal vector based on time offset
//Code written by Michal Wiatrowski, code adapted based on an openGL shader
//original author: //https://learnopengl.com/Advanced-OpenGL/Geometry-Shader

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
    float time;
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

//Main geometry shader function
[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
    //New position float
    float4 newPosition;

    //Calculate the new normal vector by using the cross product and getting the perpendicular vector; the normal vector
    float3 a = input[0].position - input[1].position;
    float3 b = input[2].position - input[1].position;
    float3 N = normalize(cross(a, b));

    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[0].position + float4(N * ((sin(time) + 1.0)), 0.0f);
    //Set the new normal
    output.normal = N;
    //Don't apply any texture coordinate calculations
    output.tex = input[0].tex;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the new vertice as viewed by the light sources
    output.lightViewPos = mul(newPosition, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix[0]);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix[0]);

    output.lightViewPos2 = mul(newPosition, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrix[1]);
    output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrix[1]);

    
    output.lightViewPos3 = mul(newPosition, worldMatrix);
    output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix[2]);
    output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix[2]);

    //Calculate the world position of the new vertex
    float4 worldPosition = mul(newPosition, worldMatrix);
    
    //Calculate the view vector between the camera position and the world position of the vertex
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);

    //pass through the world position 
    output.worldPosition = worldPosition.xyz;

    //Output the vertex and move onto the next one and repeat the process
    triStream.Append(output);


    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[1].position + float4(N * ((sin(time) + 1.0)), 0.0f);
    //Set the new normal
    output.normal = N;
    //Don't apply any texture coordinate calculations
    output.tex = input[1].tex;

     // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the new vertice as viewed by the light sources
    output.lightViewPos = mul(newPosition, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix[0]);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix[0]);

    output.lightViewPos2 = mul(newPosition, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrix[1]);
    output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrix[1]);

    output.lightViewPos3 = mul(newPosition, worldMatrix);
    output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix[2]);
    output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix[2]);

    //Calculate the world position of the new vertex
    worldPosition = mul(newPosition, worldMatrix);

    //Calculate the view vector between the camera position and the world position of the vertex
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);

    //pass through the world position 
    output.worldPosition = worldPosition.xyz;

    //Output the vertex and move onto the next one and repeat the process for the last vertex
    triStream.Append(output);


    //Offset the position of the vertice on the normal vector direction based on the offset value
    //The time is passed into the sin function which returns a value between 0 and 1, since we want to explode the object we need to add +1
    //to prevent the vertice from imploding
    newPosition = input[2].position + float4(N * ((sin(time) + 1.0)), 0.0f);
    //Set the new normal
    output.normal = N;
    //Don't apply any texture coordinate calculations
    output.tex = input[2].tex;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(newPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the new vertice as viewed by the light sources
    output.lightViewPos = mul(newPosition, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix[0]);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix[0]);

    output.lightViewPos2 = mul(newPosition, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrix[1]);
    output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrix[1]);

    output.lightViewPos3 = mul(newPosition, worldMatrix);
    output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix[2]);
    output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix[2]);

    //Calculate the world position of the new vertex
    worldPosition = mul(newPosition, worldMatrix);

    //Calculate the view vector between the camera position and the world position of the vertex
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);

    //pass through the world position 
    output.worldPosition = worldPosition.xyz;

    //Output the last vertex
    triStream.Append(output);

    //Move onto the next triangle
    triStream.RestartStrip();
}