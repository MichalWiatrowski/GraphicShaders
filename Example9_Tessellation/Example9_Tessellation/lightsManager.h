#pragma once
#include "includes.h"
//A light manager class
//Create lights here so other classes can access them
class lightsManager
{
public:
	lightsManager();
	~lightsManager();

	Light* light1;
	Light* light[3];
	Light* light2;
};

