#pragma once
//Scene manager class
//This class will be used to design the scene and send data into the shaders
//It will also hold controls for lights, post processing etc
#include "DXF.h"// include dxframework
#include "meshManager.h"
#include "shaders.h"
#include "lightsManager.h"
class sceneManager
{
public:
	sceneManager(XMFLOAT2 screen_Size);
	~sceneManager();

	//This function will be called in the App1.cpp that will pass in pointers for other managers
	void sceneManagerUpdate(D3D * renderer, shaders * Shaders, meshManager * Meshes, Camera * camera, TextureManager * textureMgr, lightsManager * LightsManager, XMFLOAT2 mousePos, float deltaTime, bool* wireframeToggle);
private:
	void depthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager);
	void secondDepthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager);
	void thirdDepthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager);
	void finalPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, Camera* camera, TextureManager* textureMgr, lightsManager* LightsManager, bool* wireframeToggle);
	void postProcess(D3D* renderer, shaders* Shaders, meshManager* Meshes, Camera* camera, XMFLOAT2 mousePos);

	void downSample();
	void upSample();

	//Render ImGui
	void gui(D3D * renderer, bool* wireframeToggle);
	//Update lights direction, position and other user defined variables that control the scene
	void updateObjects(lightsManager* LightsManager);

private:
	//Stores the screen size that will be passed in when the constructor is called
	XMFLOAT2 screenSize;
	
	//init variables for controlling the tessallation of the height map and the sin wave
	bool heightMapHardDistance = false;
	bool sinWaveHardDistance = false;
	float planeID = 1;
	XMFLOAT2 minMaxTessFactor = { 1, 1 };

	//variables for the movement of the sine wave
	float speed = 1;
	float height = 1;
	float time = 0;

	//Light controls
	XMFLOAT3 light1Controls = { -1.0f, -1.0f, 1.0f };
	XMFLOAT3 spotlightControls = { 10.0f, 10.0f, 100.0f };
	XMFLOAT3 leftLightDirection = { -1.f, -0.9f, 0.01f };
	XMFLOAT3 rightLightDirection = { 1.f, -0.9f, 0.01f };

	//init variables that control post processing
	int postProcessType = 1;
	XMFLOAT2 blurDirection = { 0, 0 };
	float blurDistance = 0;
};

