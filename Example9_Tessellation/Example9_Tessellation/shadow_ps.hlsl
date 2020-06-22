//Shadow pixel shader
//The shadow shader has 3 jobs; calculate shadows, apply lightning with directional lights and spotlight and calculate specular factor of cook torrance
//The code is written by Michal Wiatrowski, adapted from the following sources:
//http://www.rastertek.com/dx11tut40.html //shadows (shadow code provided by Paul Robertson in a lecture but more research done in here)
//http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx //cook torrance
//https://www.gamedev.net/forums/topic/545762-pixel-shader-spotlight-with-hlsl/ //spotlight
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture2 : register(t2);
Texture2D depthMapTexture3 : register(t3);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

//Declare constant buffers
cbuffer LightBuffer : register(b0)
{
	float4 ambient[3];
	float4 diffuse[3];
	float4 direction[3];
    float4 spotlightPosition;
};

struct InputType
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
//GGX microfacet distribution
//Used to describe the the brightness at a calculated point
float GGX_Distribution(float3 n, float3 h, float alpha)
{
    int PI = 3.14159265359;
    float NoH = dot(n, h);
    float alpha2 = alpha * alpha;
    float NoH2 = NoH * NoH;
    float den = NoH2 * alpha2 + (1 - NoH2);
    return (saturate(NoH) * alpha2) / (PI * den * den);
}

//GGX geometry function
//Used to describe the attenuation of the light due
float GGX_PartialGeometryTerm(float3 v, float3 n, float3 h, float alpha)
{
    float VoH2 = saturate(dot(v, h));
    float chi = saturate(VoH2 / saturate(dot(v, n)));
    VoH2 = VoH2 * VoH2;
    float tan2 = (1 - VoH2) / VoH2;
    return (chi * 2) / (1 + sqrt(1 + alpha * alpha * tan2));
}


//calculate fresnel using shlicks approximation
float3 Fresnel_Schlick(float cosT, float3 F0)
{
    return F0 + (1 - F0) * pow(1 - cosT, 5);
}

//calculate cook torrance 
float4 calcCookTorrance(float3 lightDirection, float3 normal, float3 viewVector, float4 textureColour)
{
    float roughness = 0.1;
	//calculate products and the half vector
    float3 halfwayVector = normalize(lightDirection + viewVector);
    float VdotN = dot(viewVector, normal); 
    float LdotN = dot(lightDirection, normal);
    float NdotH = dot(normal, halfwayVector);
    float VdotH = dot(viewVector, halfwayVector);

	//calculate the fresnel angle
    float f_zero = abs((1 - (1 + roughness)) / (1 + (1 + roughness)));
   
    f_zero = pow(f_zero, 2);
    //blend the fresnel angle and texture colour together
    f_zero = lerp(f_zero, textureColour, roughness);
	//calculate fresnel
    float f = Fresnel_Schlick(saturate(VdotH), f_zero);

	//calculate the geometry attenuation using the GGX approximation
    float G = GGX_PartialGeometryTerm(viewVector, normal, halfwayVector, roughness) * GGX_PartialGeometryTerm(viewVector, normal, halfwayVector, roughness);
	
	//calculate the disribution factor using the ggs approximation
    float D = GGX_Distribution(normal, halfwayVector, roughness);
	
	//calculate the denominator 
    float denom = saturate(4 * (VdotN * saturate(dot(halfwayVector, normal)) + 0.05));
   
	//return the specular value of the cook torrance shader
    return saturate((G * f * D) / denom);
}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

//Main pixel shader 
float4 main(InputType input) : SV_TARGET
{
    float depthValue;
    float lightDepthValue; 
    float spotlight;

    float4 colour[3] = { float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 0.f, 1.f) };
    float4 specular[3] = { float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 0.f, 1.f) };
    //Set the shadow bias correction value
    float shadowMapBias = 0.005f;
   
    //Sample the texture
    float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);

//First light -////////////////////////////////////////////////////////////////////////////
    
	// Calculate the projected texture coordinates for the first light.
    float2 pTexCoord = input.lightViewPos.xy / input.lightViewPos.w;
    pTexCoord *= float2(0.5, -0.5);
    pTexCoord += float2(0.5f, 0.5f);
	
    // Sample the shadow map (get depth of geometry)
    depthValue = depthMapTexture.Sample(shadowSampler, pTexCoord).r;

	// Calculate the depth from the light.
    lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
    //Subtract the bias from the light depth
    lightDepthValue -= shadowMapBias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        //calculate the light intensity
        colour[0] = calculateLighting(-direction[0].xyz, input.normal, diffuse[0]);
        //calculate the specular cook-torrance factor
        specular[0] = calcCookTorrance(-direction[0].xyz, input.normal, input.viewVector, textureColour);
    }
    //Calculate the final colour for this light by adding the ambient to it
    colour[0] = saturate(colour[0] + ambient[0]);
         
//Second light -////////////////////////////////////////////////////////////////////////////    

    // Calculate the projected texture coordinates for the second light
    float2 pTexCoord2 = input.lightViewPos2.xy / input.lightViewPos2.w;
    pTexCoord2 *= float2(0.5, -0.5);
    pTexCoord2 += float2(0.5f, 0.5f);


    // Sample the second shadow map (get depth of geometry)
    depthValue = depthMapTexture2.Sample(shadowSampler, pTexCoord2).r;

	// Calculate the depth from the second light.
    lightDepthValue = input.lightViewPos2.z / input.lightViewPos2.w;
    lightDepthValue -= shadowMapBias;

    // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        //calculate the light intensity
        colour[1] = calculateLighting(-direction[1].xyz, input.normal, diffuse[1]);
        //calculate the specular cook-torrance factor
        specular[1] = calcCookTorrance(-direction[1].xyz, input.normal, input.viewVector, textureColour);
    }
    
    colour[1] = saturate(colour[1] + ambient[1]);

//third light (spotlight) -////////////////////////////////////////////////////////////////////////////   
    
 // Calculate the projected texture coordinates for spotlight
    float2 pTexCoord3 = input.lightViewPos3.xy / input.lightViewPos3.w;
    pTexCoord3 *= float2(0.5, -0.5);
    pTexCoord3 += float2(0.5f, 0.5f);


    // Sample the second shadow map (get depth of geometry)
    depthValue = depthMapTexture3.Sample(shadowSampler, pTexCoord3).r;

	// Calculate the depth from the second light.
    lightDepthValue = input.lightViewPos3.z / input.lightViewPos3.w;
    lightDepthValue -= shadowMapBias;


    //calculate the light vector from the spotlight to the position of the vertex
    float3 lightVector = normalize(spotlightPosition.xyz - input.worldPosition);
  
    //calculate the distance from the vertex and the spotlight
    float distance = length(spotlightPosition.xyz - input.worldPosition);
  
    //calculate the perpendicular vector of the normal and light vector
    float NdotL = max(0.0f, dot(input.normal, lightVector));

    //calculate the spotlight angle
    float cosAngle = max(0.0f, dot(-direction[2].xyz, lightVector));

	//maybe add object light view TODO-----------------------------------------------------------------------------------
	
    //if the angle is greater than 30 do lightning and specular calculations
    if (cosAngle > radians(30))
    {
		//calculate the spotlight value
        spotlight = smoothstep(30, 10, cosAngle);
        if (lightDepthValue < depthValue)
        {
            //calculate the light intensity
            colour[2] = calculateLighting(-direction[2].xyz, input.normal, diffuse[2]);
             //calculate the specular cook-torrance factor
            specular[2] = calcCookTorrance(-direction[2].xyz, input.normal, lightVector, textureColour);
        }

    }
    //otherwise if smaller than 10
    else if (cosAngle < radians(10))
        //spotlight value is 1
        spotlight = 1.0f;
    else
        //and lastly anything else have spotlight value be 0
        spotlight = 0.0f;
	

    //calculate the spotlight colour in the cone, and add ambient to get the final colour
    colour[2] = saturate(float4(colour[2] * spotlight * NdotL)) + ambient[2];
 
    //checks all shadow maps if the area should not be lit
    if ((pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f) && (pTexCoord2.x < 0.f || pTexCoord2.x > 1.f || pTexCoord2.y < 0.f || pTexCoord2.y > 1.f) && (pTexCoord3.x < 0.f || pTexCoord3.x > 1.f || pTexCoord3.y < 0.f || pTexCoord3.y > 1.f))
        return textureColour;

    //saturate all the colours and specular factors together and times it by the texture colour to get the colour for the pixel
    return saturate(colour[0] + colour[1] + colour[2]) * textureColour + saturate(specular[0] + specular[1] + specular[2]);
}