//Post processing pixel shader
//Receive the renderered scene as a 2D texture, apply post processing
//and then return the colour and output it to the ortho mesh
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

//Decalre constant buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ScreenSizeBuffer : register(b1)
{
    float2 screen_size;
    float2 mousePos;
    float postProcessType;
    float horizontal;
    float2 padding2;
};

cbuffer DirectionalBlurBuffer : register(b2)
{
    float2 direction;
    float distance;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//Apply black and white or gray post processing outwith the circle based around the mouse position
float4 mousePointerPostProccessing(float4 colour, float2 tex)
{
    //Calculate the black and white based on the current pixel
    float grayscale = dot(float3(0.25, 0.65, 0.1), colour.rgb);
    //set the new colour to the current pixel
    colour.rgb = grayscale;

    //Store the colour gray in a float4
    float4 pureGray = float4(0.2f, 0.2f, 0.2f, 1.0f);

    //Get and store the mouse position in a new float2
    float2 mousePosition = mousePos;
     
    //The mouse position is already in screen coordinates, so now all we have to do is divide it by the screen size to clamp it betweeen 0 and 1
    mousePosition /= screen_size;

    //Calculate the circle around the mouse position using the equation of a circle = c^2 = a^2 + b^2
    //Code written by Michal Wiatrowski, adapted from the following stack overflow answer
    ////https://stackoverflow.com/questions/20225925/draw-a-circle-with-hlsl-and-some-math

    //We need to divide the x texture coordinates and the x mouse position by the screen size so the circle is drawn correcly based on the ration of the screen rather than an oval
    float dx = pow((tex.x * screen_size.x / screen_size.y) - mousePosition.x * screen_size.x / screen_size.y, 2);
    float dy = pow(tex.y - mousePosition.y, 2);
    
    //Calculate the circle based on c^2 = a^2 + b^2
    float circle = dx + dy;

    // any pixel within the circle return normal colour 
    if (circle < 0.005f) 
        return shaderTexture.Sample(SampleType, tex);
    else
    {
        //Return black and white colour if black and white mode is selected
        if (postProcessType == 2)
            return colour;
        //Otherwise return the colour of the gray
        else
            return pureGray;
    }
}

//Apply bloom effect depending on the brightness level of the pixel
float4 bloomEffect(float4 colour, float2 tex)
{
    //Code written by Michal Wiatrowski, original author: https://learnopengl.com/Advanced-Lighting/Bloom

    //Calculate the brightness colour of the pixel
    float brightness = dot(colour.rgb, float3(0.2126, 0.7152, 0.0722));

    //If the brightness passes the threshold, apply gaussian blur to the brightness value
    //Gaussian blur code provided by Paul Roberston 
    if (brightness > 1.0)
    {
        // Create the weights that each neighbor pixel will contribute to the blur.
        float weight0 = 0.382928;
        float weight1 = 0.241732;
        float weight2 = 0.060598;
        float weight3 = 0.005977;
        float weight4 = 0.000229;

        // Initialize the colour to black.
        colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

        if (horizontal == 0)
        {
            
            float texelSize = 1.0f / screen_size.x;
             // Add the horizontal pixels to the colour by the specific weight of each.
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * -4.0f, 0.0f)) * weight4;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * -3.0f, 0.0f)) * weight3;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * -2.0f, 0.0f)) * weight2;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * -1.0f, 0.0f)) * weight1;

            colour += shaderTexture.Sample(SampleType, tex) * weight0;

            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * 1.0f, 0.0f)) * weight1;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * 2.0f, 0.0f)) * weight2;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * 3.0f, 0.0f)) * weight3;
            colour += shaderTexture.Sample(SampleType, tex + float2(texelSize * 4.0f, 0.0f)) * weight4;
        }
        else
        {
            float texelSize = 1.0f / screen_size.y;
            // Add the vertical pixels to the colour by the specific weight of each.
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * -4.0f)) * weight4;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * -3.0f)) * weight3;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * -2.0f)) * weight2;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * -1.0f)) * weight1;
   
            colour += shaderTexture.Sample(SampleType, tex) * weight0;

            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * 1.0f)) * weight1;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * 2.0f)) * weight2;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * 3.0f)) * weight3;
            colour += shaderTexture.Sample(SampleType, tex + float2(0.0f, texelSize * 4.0f)) * weight4;
        }

       // Set the alpha channel to one.
        colour.a = 1.0f;

        //Take the brightness colour, add it with the original colour and return it
        float4 finalColour = colour + shaderTexture.Sample(SampleType, tex);
        return finalColour;
    }
    //If the brightness threshold is not met, just return the colour of the pixel without any calculations
    else
        return colour;
}

//Apply directional blur depending on the direction by adding the offset pixel colour to the current colour
float4 directionalBlur(float4 colour, float2 tex)
{
    //code adapted from https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch27.html

    //Repeat 100 times
    for (int i = 1; i < 100; i++)
    {
       //Increment the offset
       tex += direction * (distance / 100);
       
       //Add the offset to the current pixel colour
       colour += shaderTexture.Sample(SampleType, tex);
    }
    //Average the colour samples to get the final colour
    colour = colour / 100;
    //clamp and return the final colour
    return saturate(colour);
}

//Main pixel shader function
float4 main(InputType input) : SV_TARGET
{
    float4 colour;

    //Sample the texture
    colour = shaderTexture.Sample(SampleType, input.tex);

    //If 1, do not apply any post processing
    if (postProcessType == 1)
    {
        return colour;
    }

    //If mode 2 or 3 is selected, pass through colour and texture coordinates to the mouse pointer funtion
    else if (postProcessType == 2 || postProcessType == 3)
    {
        //output returned colour
        return mousePointerPostProccessing(colour, input.tex);

    }

    //If mode is 4, apply directional blur
    else if(postProcessType == 4)
    {
        //output returned colour
        return directionalBlur(colour, input.tex);
    }

    //If mode is 5, apply the bloom effect
    else if (postProcessType == 5)
    {
        //output return colour
        return bloomEffect(colour, input.tex);

    }

//default return:
    return colour;
}