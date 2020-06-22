#pragma once
#include "includes.h"
//Manager class for meshes, allow other classes to access them freely
class meshManager
{
public:
	meshManager(D3D* renderer, HWND hwnd, XMFLOAT2 screenSize);
	~meshManager();

	//for shadow demo (main scene)
	PlaneMesh* planeMesh; //simple plane mesh
	Model* teaPotModel;
	Model* testModel;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	SphereMesh* sphereMeshTest; //exploding sphere

	SphereMesh* sphereMesh2; //delete?

	//tessallated planes
	tessPlaneMesh* heightMapMesh;
	tessPlaneMesh* sinWaveMesh;

	//billboards
	NewPointMesh* pointMesh; //sun1
	NewPointMesh* sun2;

	//Render textures for storing the depth maps
	RenderTexture* shadowMap;
	RenderTexture* shadowMap2;
	RenderTexture* shadowMap3;

	//render texture and ortho mesh for post processing
	RenderTexture* blurProcessTexture;
	RenderTexture* postProcessTexture;
	OrthoMesh* postProcessOrthoMesh;
};

