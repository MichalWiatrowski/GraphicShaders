#include "lightsManager.h"



lightsManager::lightsManager()
{
	//Setup light for the height map and the sin wave 
	light1 = new Light;
	light1->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light1->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light1->setDirection(1.0f, -0.4f, 0.01f);

	light2 = new Light;
	light2->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light2->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light2->setDirection(1.0f, 0.01f, 0.01f);

	//Specify the scene width and height for the ortho matrix of the lights
	int sceneWidth = 100;
	int sceneHeight = 100;

	//Create lights for the main scene
	//Green light on the right side of the scene
	light[0] = new Light;
	light[0]->setAmbientColour(0.00f, 0.00f, 0.00f, 1.0f);
	light[0]->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	light[0]->setDirection(-1.f, -0.9f, 0.01f);
	light[0]->setPosition(10.f, 10.f, 100.f);
	light[0]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 150.f);

	//Red light on the left side of the scene
	light[1] = new Light;
	light[1]->setAmbientColour(0.00f, 0.00f, 0.00f, 1.0f);
	light[1]->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light[1]->setDirection(1.0f, -0.9f, 0.01f);
	light[1]->setPosition(-10.f, 10.f, 100.f);
	light[1]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 150.f);

	//White spotlight
	light[2] = new Light;
	light[2]->setAmbientColour(0.00f, 0.00f, 0.00f, 1.0f);
	light[2]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light[2]->setDirection(0.01f, -1.0f, 0.01f);
	light[2]->setPosition(10.f, 10.f, 100.f);
	light[2]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 150.f);
}

//deconstructor for lights
lightsManager::~lightsManager()
{
	//clean up lights
	light1 = NULL;
	delete light1;

	light2 = NULL;
	delete light2;

	for (int i = 0; i < 3; i++)
	{
		light[i] = NULL;
		delete light[i];
	}
}
