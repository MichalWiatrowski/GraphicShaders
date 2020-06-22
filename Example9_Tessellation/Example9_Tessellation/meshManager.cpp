#include "meshManager.h"

//Init the mesh pointers
meshManager::meshManager(D3D * renderer, HWND hwnd, XMFLOAT2 screenSize)
{
	//tessallated plane meshes
	heightMapMesh = new tessPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100);
	sinWaveMesh = new tessPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100);

	//billboards
	pointMesh = new NewPointMesh(renderer->getDevice(), renderer->getDeviceContext(), 1);
	sun2 = new NewPointMesh(renderer->getDevice(), renderer->getDeviceContext(), 1);

	//simple plane mesh for the demo
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	//tea pot models
	teaPotModel = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");
	testModel = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");

	//meshes for shadow demo
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMeshTest = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext()); //exploding sphere

	//ortho mesh and render texture for post processing
	postProcessOrthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenSize.x, screenSize.y);	// Full screen size
	blurProcessTexture = new RenderTexture(renderer->getDevice(), screenSize.x, screenSize.y, SCREEN_NEAR, SCREEN_DEPTH);
	postProcessTexture = new RenderTexture(renderer->getDevice(), screenSize.x, screenSize.y, SCREEN_NEAR, SCREEN_DEPTH);

	//shadow map resoultions
	int shadowmapWidth = 4096;
	int shadowmapHeight = 4096;

	//shadow render targets
	shadowMap = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 170.f);
	shadowMap2 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 170.f);
	shadowMap3 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 170.f);
}

meshManager::~meshManager()
{
	heightMapMesh->~tessPlaneMesh();
	heightMapMesh = NULL;
	delete heightMapMesh;

	sinWaveMesh->~tessPlaneMesh();
	sinWaveMesh = NULL;
	delete sinWaveMesh;

	pointMesh->~NewPointMesh();
	pointMesh = NULL;
	delete pointMesh;

	sun2->~NewPointMesh();
	sun2 = NULL;
	delete sun2;

	planeMesh->~PlaneMesh();
	planeMesh = NULL;
	delete planeMesh;

	teaPotModel->~Model();
	teaPotModel = NULL;
	delete teaPotModel;

	testModel->~Model();
	testModel = NULL;
	delete testModel;

	cubeMesh->~CubeMesh();
	cubeMesh = NULL;
	delete cubeMesh;

	sphereMesh->~SphereMesh();
	sphereMesh = NULL;
	delete sphereMesh;

	sphereMeshTest->~SphereMesh();
	sphereMeshTest = NULL;
	delete sphereMeshTest;

	postProcessOrthoMesh->~OrthoMesh();
	postProcessOrthoMesh = NULL;
	delete postProcessOrthoMesh;

	blurProcessTexture->~RenderTexture();
	blurProcessTexture = NULL;
	delete blurProcessTexture;

	postProcessTexture->~RenderTexture();
	postProcessTexture = NULL;
	delete postProcessTexture;

	shadowMap->~RenderTexture();
	shadowMap = NULL;
	delete shadowMap;

	shadowMap2->~RenderTexture();
	shadowMap2 = NULL;
	delete shadowMap2;

	shadowMap3->~RenderTexture();
	shadowMap3 = NULL;
	delete shadowMap3;
}
