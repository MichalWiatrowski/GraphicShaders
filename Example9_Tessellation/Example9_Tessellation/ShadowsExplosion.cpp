//ShadowsExplosion.cpp
#include "ShadowsExplosion.h"

//init shader and load hlsl shaders
ShadowsExplosion::ShadowsExplosion(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"shadowGeoShader_vs.cso", L"explosion_gs.cso", L"shadow_ps.cso");
}


//Release buffers and states
ShadowsExplosion::~ShadowsExplosion()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	if (sampleStateShadow)
	{
		sampleStateShadow->Release();
		sampleStateShadow = 0;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}
	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ShadowsExplosion::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	//Create buffer descriptions
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);
	
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Sampler for shadow map sampling.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);

	// Setup light buffer
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	//create buffer
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	// Setup the camera buffer
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	//create buffer
	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);
}

void ShadowsExplosion::initShader(WCHAR * vsFilename, WCHAR* gsFilename, WCHAR * psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);
	// Load other required shaders.
	loadGeometryShader(gsFilename);
}


void ShadowsExplosion::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, ID3D11ShaderResourceView*depthMap3, Light* light[], XMFLOAT3 cameraPosition, float deltaTime)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);


	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview; //viewMatrix
	dataPtr->projection = tproj; //projectionMatrix
	//for each light
	for (int i = 0; i < 3; i++)
	{
		//get and store the light view and projection matrix 
		XMMATRIX tLightViewMatrix = XMMatrixTranspose(light[i]->getViewMatrix());
		XMMATRIX tLightProjectionMatrix = XMMatrixTranspose(light[i]->getOrthoMatrix());

		dataPtr->lightView[i] = tLightViewMatrix;
		dataPtr->lightProjection[i] = tLightProjectionMatrix;
	}
	//unlock the buffer
	deviceContext->Unmap(matrixBuffer, 0);
	//Write to the constant buffer
	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional buffers
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	//for each light
	for (int i = 0; i < 3; i++)
	{
		//get the ambient, diffuse and direction
		//make the direction float4 which saves making a padding float
		lightPtr->ambient[i] = light[i]->getAmbientColour();
		lightPtr->diffuse[i] = light[i]->getDiffuseColour();
		lightPtr->direction[i].x = light[i]->getDirection().x;
		lightPtr->direction[i].y = light[i]->getDirection().y;
		lightPtr->direction[i].z = light[i]->getDirection().z;
		lightPtr->direction[i].w = 1;
	}
	//for the spotlight get the position
	lightPtr->spotlightPosition.x = light[2]->getPosition().x;
	lightPtr->spotlightPosition.y = light[2]->getPosition().y;
	lightPtr->spotlightPosition.z = light[2]->getPosition().z;
	lightPtr->spotlightPosition.w = 1;
	//unlock the buffer
	deviceContext->Unmap(lightBuffer, 0);
	//write to the constant buffer in the pixel shader
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	//Camera Buffer
	CameraBufferType* cameraPtr;
	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->cameraPosition = cameraPosition;
	cameraPtr->time = deltaTime;
	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->GSSetConstantBuffers(1, 1, &cameraBuffer);

	// Set shader texture and shadow resources in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMap);
	deviceContext->PSSetShaderResources(2, 1, &depthMap2);
	deviceContext->PSSetShaderResources(3, 1, &depthMap3);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);
}

