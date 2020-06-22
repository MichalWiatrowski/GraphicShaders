// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
}

//init all classes and load textures
void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//store the screen width and height in a float 2
	XMFLOAT2 screenSize = { float(screenWidth), float(screenHeight) };

	//shaders manager
	Shaders = new shaders(renderer, hwnd);
	
	//mesh manager
	MeshManager = new meshManager(renderer, hwnd, screenSize);
	
	//scene manager
	SceneManager = new sceneManager(screenSize);

	LightsManager = new lightsManager();
	
	//load in all the textures 
	textureMgr->loadTexture("height", L"res/height.png");
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("grass", L"res/grass.png");
	textureMgr->loadTexture("water", L"res/water.jpg");
	textureMgr->loadTexture("sun", L"res/sun.png");
}

//Release class objects 
App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	//Release manager objects
	Shaders->~shaders();
	Shaders = NULL;
	delete Shaders;

	MeshManager->~meshManager();
	MeshManager = NULL;
	delete MeshManager;

	SceneManager->~sceneManager();
	SceneManager = NULL;
	delete SceneManager;

	LightsManager->~lightsManager();
	LightsManager = NULL;
	delete LightsManager;
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	//begin render
	renderer->beginScene(0.09f, 0.18f, 0.52f, 1.0f);

	//store the mouse position
	mousePos = XMFLOAT2(input->getMouseX(), input->getMouseY());

	//call and pass in pointers to the shader, light and mesh managers
	SceneManager->sceneManagerUpdate(renderer, Shaders, MeshManager, camera, textureMgr, LightsManager, mousePos, timer->getTime(), &wireframeToggle);

	//end render
	renderer->endScene();
	return true;
}

