#pragma once
#include "includes.h"
class shaders
{
public:
	shaders(D3D* renderer, HWND hwnd);
	~shaders();

	//Create pointers for all the shader classes that are going to be used in the scene
	

	TessellationShader* tessellationShader; //tessellation shader
	GeometryShader* geometryShader; //shader for making billboards

	ShadowsExplosion* explosions; //shader with shadows, cook-torrance and explosion effect using geometry shader
	ShadowShader* shadowShader; //shader with shadows and cook torrance
	DepthShader* depthShader; //depth shader
	DepthShaderGeo* depthShaderGeo; //depth shader; slightly different as needs to pass data into the geometry shader

	postProcessShader* PostProcessShader; //shader for post processing

};

