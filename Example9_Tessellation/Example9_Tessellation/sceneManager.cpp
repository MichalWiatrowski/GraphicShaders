#include "sceneManager.h"

//When the constructor is called; receive and store screen size
sceneManager::sceneManager(XMFLOAT2 screen_Size)
{
	screenSize = screen_Size;
}

//Run the deconstructor
sceneManager::~sceneManager()
{
}

//Calculate and store the depth map for the green light in a render texture
void sceneManager::depthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager)
{
	// Set the render target to be the render to texture.
	Meshes->shadowMap->setRenderTarget(renderer->getDeviceContext());
	//Clear the render target
	Meshes->shadowMap->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Generate a light view matrix
	LightsManager->light[0]->generateViewMatrix();
	//Get and store the world matrix and the light view and ortho matrix
	XMMATRIX lightViewMatrix = LightsManager->light[0]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = LightsManager->light[0]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	XMMATRIX scaleMatrix; //Create a scale matrix 

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, 50.f);
	// Render floor
	Meshes->planeMesh->sendData(renderer->getDeviceContext()); //Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix); //Pass in data for the depth shader
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->planeMesh->getIndexCount()); //Render the mesh

	//Render cube
	worldMatrix = XMMatrixTranslation(-15.f, 2.f, 60.f);
	Meshes->cubeMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->cubeMesh->getIndexCount());

	//Render sphere
	worldMatrix = XMMatrixTranslation(-0.f, 2.f, 25.f);
	scaleMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	Meshes->sphereMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->sphereMesh->getIndexCount());

	//Render exploding sphere
	worldMatrix = XMMatrixTranslation(-10.f, 5.f, 100.f);
	Meshes->sphereMeshTest->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->sphereMeshTest->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 130.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	//Render exploding tea pot
	Meshes->testModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->testModel->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 170.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render tea pot model
	Meshes->teaPotModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->teaPotModel->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

//Calculate and store the depth map for the red light in a render texture
void sceneManager::secondDepthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager)
{
	// Set the render target to be the render to texture.
	Meshes->shadowMap2->setRenderTarget(renderer->getDeviceContext());
	//Clear the render target
	Meshes->shadowMap2->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Generate a light view matrix
	LightsManager->light[1]->generateViewMatrix();
	//Get and store the world matrix and the light view and ortho matrix
	XMMATRIX lightViewMatrix = LightsManager->light[1]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = LightsManager->light[1]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX scaleMatrix; //Create a scale matrix 


	worldMatrix = XMMatrixTranslation(-50.f, 0.f, 50.f);
	// Render floor
	Meshes->planeMesh->sendData(renderer->getDeviceContext()); //Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix); //Pass in data for the depth shader
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->planeMesh->getIndexCount()); //Render the mesh

	//Render cube
	worldMatrix = XMMatrixTranslation(-15.f, 2.f, 60.f);
	Meshes->cubeMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->cubeMesh->getIndexCount());

	//Render sphere
	worldMatrix = XMMatrixTranslation(-0.f, 2.f, 25.f);
	scaleMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	Meshes->sphereMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->sphereMesh->getIndexCount());

	//Render exploding sphere
	worldMatrix = XMMatrixTranslation(-10.f, 5.f, 100.f);
	Meshes->sphereMeshTest->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->sphereMeshTest->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 130.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	//Render exploding tea pot
	Meshes->testModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->testModel->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 170.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render tea pot model
	Meshes->teaPotModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->teaPotModel->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

//Calculate and store the depth map for the spotlight in a render texture
void sceneManager::thirdDepthPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, lightsManager* LightsManager)
{
	// Set the render target to be the render to texture.
	Meshes->shadowMap3->setRenderTarget(renderer->getDeviceContext());
	//Clear the render target
	Meshes->shadowMap3->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Generate a light view matrix
	LightsManager->light[2]->generateViewMatrix();
	//Get and store the world matrix and the light view and ortho matrix
	XMMATRIX lightViewMatrix = LightsManager->light[2]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = LightsManager->light[2]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX scaleMatrix; //Create a scale matrix 


	worldMatrix = XMMatrixTranslation(-50.f, 0.f, 50.f);
	// Render floor
	Meshes->planeMesh->sendData(renderer->getDeviceContext()); //Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix); //Pass in data for the depth shader
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->planeMesh->getIndexCount()); //Render the mesh

	//Render cube
	worldMatrix = XMMatrixTranslation(-15.f, 2.f, 60.f);
	Meshes->cubeMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->cubeMesh->getIndexCount());

	//Render sphere
	worldMatrix = XMMatrixTranslation(-0.f, 2.f, 25.f);
	scaleMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	Meshes->sphereMesh->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->sphereMesh->getIndexCount());

	//Render exploding sphere
	worldMatrix = XMMatrixTranslation(-10.f, 5.f, 100.f);
	Meshes->sphereMeshTest->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->sphereMeshTest->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 130.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	//Render exploding tea pot
	Meshes->testModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShaderGeo->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, 1, time);
	Shaders->depthShaderGeo->render(renderer->getDeviceContext(), Meshes->testModel->getIndexCount());

	worldMatrix = XMMatrixTranslation(0.f, 10.f, 170.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render tea pot model
	Meshes->teaPotModel->sendData(renderer->getDeviceContext());//Send data to the GPU
	Shaders->depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	Shaders->depthShader->render(renderer->getDeviceContext(), Meshes->teaPotModel->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

//Render the final scene, with all the shaders and post processing
void sceneManager::finalPass(D3D* renderer, shaders* Shaders, meshManager* Meshes, Camera* camera, TextureManager* textureMgr, lightsManager* LightsManager, bool* wireframeToggle)
{
	//if wireframe mode is not enabled, render the scene to a render texture for post processing
	//if not done this way when wireframe is enabled it will just show the wireframe of the ortho mesh
	if (!*wireframeToggle)
	{
		Meshes->postProcessTexture->setRenderTarget(renderer->getDeviceContext());
		Meshes->postProcessTexture->clearRenderTarget(renderer->getDeviceContext(), 0.09f, 0.18f, 0.52f, 1.0f);
	}

	//update the camera
	camera->update();

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	XMMATRIX scaleMatrix; //create a scale matrix

	/*Objects rendered through basic shadow shader-------------------------------*/

	// Render floor
	worldMatrix = XMMatrixTranslation(-50.f, 1.f, 50.f); //move the object in the scene
	
	Meshes->planeMesh->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights and camera position to the shader
	Shaders->shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition());
	Shaders->shadowShader->render(renderer->getDeviceContext(), Meshes->planeMesh->getIndexCount()); //render the object


	// Render the tea pot model
	worldMatrix = XMMatrixTranslation(0.f, 10.f, 170.f); //move the object in the scene
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f); //scale the object 
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix); //update the world matrix

	Meshes->teaPotModel->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights and camera position to the shader
	Shaders->shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition());
	Shaders->shadowShader->render(renderer->getDeviceContext(), Meshes->teaPotModel->getIndexCount()); //render the object

	//Render the cube mesh
	worldMatrix = XMMatrixTranslation(-15.f, 2.f, 60.f);//move the object in the scene

	Meshes->cubeMesh->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights and camera position to the shader
	Shaders->shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition());
	Shaders->shadowShader->render(renderer->getDeviceContext(), Meshes->cubeMesh->getIndexCount()); //render the object

	//Render the sphere
	worldMatrix = XMMatrixTranslation(0.f, 2.f, 25.f); //move the object in the scene
	scaleMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f); //scale the object 
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix); //update the world matrix
	
	Meshes->sphereMesh->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights and camera position to the shader
	Shaders->shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition());
	Shaders->shadowShader->render(renderer->getDeviceContext(), Meshes->sphereMesh->getIndexCount()); //render the object


	/*Objects rendered through explosion geometry shader with shadows-------------------------------*/

	//exploding sphere
	worldMatrix = XMMatrixTranslation(-10.f, 5.f, 100.f);  //move the object in the scene

	Meshes->sphereMeshTest->sendData(renderer->getDeviceContext());//send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights, camera position to the shader and offset value
	Shaders->explosions->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition(), time);
	Shaders->explosions->render(renderer->getDeviceContext(), Meshes->sphereMeshTest->getIndexCount());  //render the object


	//Render exploding teapot
	worldMatrix = XMMatrixTranslation(0.f, 10.f, 130.f);  //move the object in the scene
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);//scale the object 
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix); //update the world matrix

	Meshes->testModel->sendData(renderer->getDeviceContext());  //send data to the GPU
	//Pass in the brick texture, the 3 shadow maps, lights, camera position to the shader and offset value
	Shaders->explosions->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), Meshes->shadowMap->getShaderResourceView(), Meshes->shadowMap2->getShaderResourceView(), Meshes->shadowMap3->getShaderResourceView(), LightsManager->light, camera->getPosition(), time);
	Shaders->explosions->render(renderer->getDeviceContext(), Meshes->testModel->getIndexCount());  //render the object


	/*Objects rendered through basic geometry shader-------------------------------*/


	//billboards
	worldMatrix = XMMatrixTranslation(-100.f, 20.f, 100.f);   //move the object in the scene

	Meshes->pointMesh->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the sun texture, the 3 shadow maps, lights and the size of the billboard
	Shaders->geometryShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("sun"), LightsManager->light2, 50);
	Shaders->geometryShader->render(renderer->getDeviceContext(), Meshes->pointMesh->getIndexCount());//render the object

	worldMatrix = XMMatrixTranslation(100.f, 20.f, 100.f);   //move the object in the scene

	Meshes->sun2->sendData(renderer->getDeviceContext()); //send data to the GPU
	//Pass in the sun texture, the 3 shadow maps, lights and the size of the billboard
	Shaders->geometryShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("sun"), LightsManager->light2, 50);
	Shaders->geometryShader->render(renderer->getDeviceContext(), Meshes->sun2->getIndexCount());//render the object


	/*Objects rendered through basic geometry shader-------------------------------*/


	//render the height map mesh
	worldMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);  //move the object in the scene
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f); //scale the object 
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix); //update the world matrix

	Meshes->heightMapMesh->sendData((renderer->getDeviceContext())); //send data to the GPU
	//Pass in the height map, texture, camera posotion, tessellation factor, boolean to check if distance based tessellation has been turned on, light and vlaues that control the sin wave
	Shaders->tessellationShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), textureMgr->getTexture("grass"), camera->getPosition(), 1, minMaxTessFactor.x, heightMapHardDistance, LightsManager->light1, time, speed, height);
	Shaders->tessellationShader->render(renderer->getDeviceContext(), Meshes->heightMapMesh->getIndexCount());//render the object


	//render the sin wave mesh
	worldMatrix = XMMatrixTranslation(0.f, 2.f, 0.f); //move the object in the scene
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);  //scale the object 
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix); //update the world matrix

	Meshes->sinWaveMesh->sendData((renderer->getDeviceContext())); //send data to the GPU
	//Pass in the height map, texture, camera posotion, tessellation factor, boolean to check if distance based tessellation has been turned on, light and vlaues that control the sin wave
	Shaders->tessellationShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), textureMgr->getTexture("water"), camera->getPosition(), 2, minMaxTessFactor.y, sinWaveHardDistance, LightsManager->light1, time, speed, height);
	Shaders->tessellationShader->render(renderer->getDeviceContext(), Meshes->sinWaveMesh->getIndexCount());//render the object


	//if wireframe is not enabled, set back buffer as render target
	if (!*wireframeToggle)
	{
		renderer->setBackBufferRenderTarget();
	}	
}
//Renders the previously renderered scene on a ortho mesh that can be post processed
void sceneManager::postProcess(D3D* renderer, shaders* Shaders, meshManager* Meshes, Camera* camera, XMFLOAT2 mousePos)
{
	//disable the z buffer
	renderer->setZBuffer(false);

	XMMATRIX worldMatrix = renderer->getWorldMatrix(); //get world matrix
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	//if bloom effect is enabled render to an extra render target for another pass (1 for horizontal and 1 for vertical)
	if (postProcessType == 5)
	{
		Meshes->blurProcessTexture->setRenderTarget(renderer->getDeviceContext());
		Meshes->blurProcessTexture->clearRenderTarget(renderer->getDeviceContext(), 0.09f, 0.18f, 0.52f, 1.0f);

		Meshes->postProcessOrthoMesh->sendData(renderer->getDeviceContext());  //send data to the GPU
		//pass in the render target and values used for post processing
		Shaders->PostProcessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, Meshes->postProcessTexture->getShaderResourceView(), screenSize, mousePos, postProcessType, blurDirection, blurDistance, 0);
		Shaders->PostProcessShader->render(renderer->getDeviceContext(), Meshes->postProcessOrthoMesh->getIndexCount());//render post processed scene

		renderer->setBackBufferRenderTarget();

		Meshes->postProcessOrthoMesh->sendData(renderer->getDeviceContext());  //send data to the GPU
		//pass in the render target and values used for post processing
		Shaders->PostProcessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, Meshes->blurProcessTexture->getShaderResourceView(), screenSize, mousePos, postProcessType, blurDirection, blurDistance, 1);
		Shaders->PostProcessShader->render(renderer->getDeviceContext(), Meshes->postProcessOrthoMesh->getIndexCount());//render post processed scene
	}
	else
	{
		Meshes->postProcessOrthoMesh->sendData(renderer->getDeviceContext());  //send data to the GPU
		//pass in the render target and values used for post processing
		Shaders->PostProcessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, Meshes->postProcessTexture->getShaderResourceView(), screenSize, mousePos, postProcessType, blurDirection, blurDistance, 0);
		Shaders->PostProcessShader->render(renderer->getDeviceContext(), Meshes->postProcessOrthoMesh->getIndexCount());//render post processed scene
	}
	

	//enable back the z buffer
	renderer->setZBuffer(true);
}

void sceneManager::sceneManagerUpdate(D3D * renderer, shaders * Shaders, meshManager * Meshes, Camera * camera, TextureManager * textureMgr, lightsManager * LightsManager, XMFLOAT2 mousePos, float deltaTime, bool* wireframeToggle)
{
	time += deltaTime; //increment the offset value based on delta time
	if (time > 600) // if value is higher than 600, set it back to 600 
		time = 0;

	updateObjects(LightsManager); //update any objects, lights etc

	depthPass(renderer,  Shaders,  Meshes,  LightsManager); //depth pass for the right light
	secondDepthPass(renderer, Shaders,  Meshes,  LightsManager); //depth pass for the left light
	thirdDepthPass(renderer,  Shaders,  Meshes,  LightsManager); //depth pass for the spotlight
	finalPass(renderer, Shaders,  Meshes, camera, textureMgr, LightsManager, wireframeToggle); //render the scene tp the render target

	 if (!*wireframeToggle) //if wireframe mode is not enabled, apply post processing
	postProcess(renderer, Shaders, Meshes, camera, mousePos); //apply post processsing to the render target

	gui(renderer, wireframeToggle); //render gui
}

//Render ImGui
void sceneManager::gui(D3D * renderer, bool* wireframeToggle)
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	
	//Controls for lights and post processing
	ImGui::Checkbox("Wireframe mode", wireframeToggle);
	ImGui::SliderInt("Post Process", &postProcessType, 1, 5);
	ImGui::SliderFloat("Height Map Tessellation", &minMaxTessFactor.x, 1, 32);
	ImGui::Checkbox("Distance/Value Tess for Height Map", &heightMapHardDistance);
	ImGui::SliderFloat("Sin wave Tessellation", &minMaxTessFactor.y, 1, 32);
	ImGui::Checkbox("Distance/Value Tess for sin wave", &sinWaveHardDistance);
	ImGui::SliderFloat("Sin wave speed", &speed, 1, 30);
	ImGui::SliderFloat("Sin wave height", &height, 1, 3);

	ImGui::SliderFloat3("Spotlight position", &spotlightControls.x, - 200, 200);
	ImGui::SliderFloat3("Left light direction", &leftLightDirection.x, -1, 1);
	ImGui::SliderFloat3("Right light direction", &rightLightDirection.x, -1, 1);
	ImGui::SliderFloat3("Height Map light direction", &light1Controls.x, -1, 1);
	ImGui::SliderFloat2("Directional Blur Direction X: Y:", &blurDirection.x, -0.5, 0.5);
	ImGui::SliderFloat("Directional Blur Distance", &blurDistance, 0, 1);


	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//Update any objects, lights etc based on the values changed in ImGui
void sceneManager::updateObjects(lightsManager * LightsManager)
{
	LightsManager->light[0]->setDirection(leftLightDirection.x, leftLightDirection.y, leftLightDirection.z);
	LightsManager->light[1]->setDirection(rightLightDirection.x, rightLightDirection.y, rightLightDirection.z);
	LightsManager->light[2]->setPosition(spotlightControls.x, spotlightControls.y, spotlightControls.z);
	LightsManager->light1->setDirection(light1Controls.x, light1Controls.y, light1Controls.z);
}
