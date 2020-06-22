//postProcessShader.cpp
#include "postProcessShader.h"

//init shader and load hlsl shaders
postProcessShader::postProcessShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"postProcess_vs.cso", L"postProcess_ps.cso");
}

//Release buffers and states
postProcessShader::~postProcessShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (screenSizeBuffer)
	{
		screenSizeBuffer->Release();
		screenSizeBuffer = 0;
	}

	if (directionalBuffer)
	{
		directionalBuffer->Release();
		directionalBuffer = 0;
	}
	//Release base shader components
	BaseShader::~BaseShader();
}

void postProcessShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	//Create buffer descriptions
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC screenSizeBufferDesc;
	D3D11_BUFFER_DESC directionalBlurBufferDesc;

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
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup screen size buffer
	//The buffer will store the screen size, mouse position and the int value which will determine which post process to run
	// Setup the description of the screen size dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	screenSizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenSizeBufferDesc.ByteWidth = sizeof(ScreenSizeBufferType);
	screenSizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenSizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenSizeBufferDesc.MiscFlags = 0;
	screenSizeBufferDesc.StructureByteStride = 0;
	//Create the buffer
	renderer->CreateBuffer(&screenSizeBufferDesc, NULL, &screenSizeBuffer);

	// Setup directional buffer
	//The buffer will store the directional blur direction and distance
	// Setup the description of thedirectional blur dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	directionalBlurBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	directionalBlurBufferDesc.ByteWidth = sizeof(DirectionalBlurBufferType);
	directionalBlurBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	directionalBlurBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	directionalBlurBufferDesc.MiscFlags = 0;
	directionalBlurBufferDesc.StructureByteStride = 0;
	//Create the buffer
	renderer->CreateBuffer(&directionalBlurBufferDesc, NULL, &directionalBuffer);

}


void postProcessShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT2 screenSize, XMFLOAT2 mousePos, float postProcess, XMFLOAT2 direction, float distance, float horizontal)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	//Lock the buffer to the directx device, write the data to the buffer to pass them into the shaders, unlock it and finally pass the data into the specified shader
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional buffers
	//Pass post processing data into the pixel shader
	ScreenSizeBufferType* screenSizePtr;
	deviceContext->Map(screenSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	screenSizePtr = (ScreenSizeBufferType*)mappedResource.pData;
	screenSizePtr->screen_size.x = screenSize.x;
	screenSizePtr->screen_size.y = screenSize.y;
	screenSizePtr->mousePos = mousePos;
	screenSizePtr->postProcessType = postProcess;
	screenSizePtr->horizontal = horizontal;
	deviceContext->Unmap(screenSizeBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &screenSizeBuffer);

	//Pass directional blur data into the pixel shader
	DirectionalBlurBufferType* blurPtr;
	deviceContext->Map(directionalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	blurPtr = (DirectionalBlurBufferType*)mappedResource.pData;
	blurPtr->direction = direction;
	blurPtr->distance = distance;
	deviceContext->Unmap(directionalBuffer, 0);
	deviceContext->PSSetConstantBuffers(2, 1, &directionalBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
