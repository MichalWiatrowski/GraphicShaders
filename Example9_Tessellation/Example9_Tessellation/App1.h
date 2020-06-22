// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "includes.h"

#include "shaders.h"
#include "meshManager.h"
#include "sceneManager.h"
#include "lightsManager.h"
class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();


private:

	shaders* Shaders;
	meshManager* MeshManager;
	sceneManager* SceneManager;
	lightsManager* LightsManager;







	XMFLOAT2 mousePos;
	

	
};

#endif