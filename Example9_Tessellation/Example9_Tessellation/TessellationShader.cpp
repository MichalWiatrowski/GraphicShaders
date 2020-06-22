// tessellation shader.cpp
#include "tessellationshader.h"

//init shader and load hlsl shaders
TessellationShader::TessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"tessellation_ps.cso");
}

//Release buffers and states
TessellationShader::~TessellationShader()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
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
	if (tessellationBuffer)
	{
		tessellationBuffer->Release();
		tessellationBuffer = 0;
	}
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}
	if (sinWaveBuffer)
	{
		sinWaveBuffer->Release();
		sinWaveBuffer = 0;
	}
	if (blendState)
	{
		blendState->Release();
		blendState = 0;
	}

	
	//Release base shader components
	BaseShader::~BaseShader();
}

void TessellationShader::initShader(WCHAR* vsFilename,  WCHAR* psFilename)
{
	//Create buffer descriptions
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC tessBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC sinWaveBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	//Create a blend state description
	//This blend state will disable alpha blending
	//This shader will be used after the billboard shader and alpha blending needs to be disabled 
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.RenderTarget[0].BlendEnable = false;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//Create the blend state
	renderer->CreateBlendState(&blendStateDesc, &blendState);

	// Setup the description of the dynamic matrix constant buffer that is in the hull and domain shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the hull and domain shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

	// Setup the tessellation buffer
	tessBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessBufferDesc.ByteWidth = sizeof(TessellationBufferType);
	tessBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessBufferDesc.MiscFlags = 0;
	tessBufferDesc.StructureByteStride = 0;
	// Create the buffer
	renderer->CreateBuffer(&tessBufferDesc, NULL, &tessellationBuffer);


	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	//create buffer
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	// Setup sin wave buffer
	sinWaveBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	sinWaveBufferDesc.ByteWidth = sizeof(SinWaveType);
	sinWaveBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sinWaveBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sinWaveBufferDesc.MiscFlags = 0;
	sinWaveBufferDesc.StructureByteStride = 0;
	//create buffer
	renderer->CreateBuffer(&sinWaveBufferDesc, NULL, &sinWaveBuffer);

}

void TessellationShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}


void TessellationShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* heightMapTexture, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPos, float planeID, float minMaxTessFactor, float hardDistance, Light* light, float deltaTime, float speed, float height)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(world);
	XMMATRIX tview = XMMatrixTranspose(view);
	XMMATRIX tproj = XMMatrixTranspose(projection);

	//Lock the buffer to the directx device, write the data to the buffer to pass them into the shaders, unlock it and finally pass the data into the specified shader
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->HSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional buffers
	//Pass tessellation data into the hull and domain shader
	result = deviceContext->Map(tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TessellationBufferType* tessPtr = (TessellationBufferType*)mappedResource.pData;
	tessPtr->cameraPos = cameraPos;
	tessPtr->planeID = planeID;
	tessPtr->minMaxTessFactor = minMaxTessFactor;
	tessPtr->hardDistance = hardDistance;
	deviceContext->Unmap(tessellationBuffer, 0);
	deviceContext->HSSetConstantBuffers(1, 1, &tessellationBuffer);
	deviceContext->DSSetConstantBuffers(1, 1, &tessellationBuffer);
	
	//Pass light buffer to the pixel shader
	LightBufferType* lightPtr;
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	//Pass sin wave data to the domain shader
	SinWaveType* sinWavePtr;
	deviceContext->Map(sinWaveBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	sinWavePtr = (SinWaveType*)mappedResource.pData;
	sinWavePtr->time_speed_height.x = deltaTime;
	sinWavePtr->time_speed_height.y = speed;
	sinWavePtr->time_speed_height.z = height;
	deviceContext->Unmap(sinWaveBuffer, 0);
	deviceContext->DSSetConstantBuffers(2, 1, &sinWaveBuffer);


	// Set shader texture resource in the pixel shader and the height map resource in the domain shader
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->DSSetShaderResources(0, 1, &heightMapTexture);
	deviceContext->DSSetSamplers(0, 1, &sampleState);

	//disable blending in the output merger
	deviceContext->OMSetBlendState(blendState, 0, 0xffffffff);
}



