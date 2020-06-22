#include "shaders.h"

//init shader pointers
shaders::shaders(D3D* renderer, HWND hwnd)
{
	//all shaders
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);

	explosions = new ShadowsExplosion(renderer->getDevice(), hwnd);
	depthShaderGeo = new DepthShaderGeo(renderer->getDevice(), hwnd);

	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);

	PostProcessShader = new postProcessShader(renderer->getDevice(), hwnd);
}

//Run deconstructor and release all shaders
shaders::~shaders()
{
	tessellationShader->~TessellationShader();
	tessellationShader = NULL;
	delete tessellationShader;

	explosions->~ShadowsExplosion();
	explosions = NULL;
	delete explosions;

	depthShaderGeo->~DepthShaderGeo();
	depthShaderGeo = NULL;
	delete depthShaderGeo;

	depthShader->~DepthShader();
	depthShader = NULL;
	delete depthShader;

	shadowShader->~ShadowShader();
	shadowShader = NULL;
	delete shadowShader;

	geometryShader->~GeometryShader();
	geometryShader = NULL;
	delete geometryShader;

	PostProcessShader->~postProcessShader();
	PostProcessShader = NULL;
	delete PostProcessShader;
}
