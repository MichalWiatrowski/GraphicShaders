// geometry shader.cpp
#include "geometryshader.h"

//init shader and load hlsl shaders
GeometryShader::GeometryShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"billboard_vs.cso", L"billboard_gs.cso", L"billboard_ps.cso");
}

//Release buffers and states
GeometryShader::~GeometryShader()
{
	//Release the sampler
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	//Release the matrix buffer
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	// Release layout component
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (sizeBuffer)
	{
		sizeBuffer->Release();
		sizeBuffer = 0;
	}
	if (blendState)
	{
		blendState->Release();
		blendState = 0;
	}
	//Release base shader components
	BaseShader::~BaseShader();
}

void GeometryShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	//Create buffer descriptions
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC sizeBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	//Create a blend state description
	//This blend state will enable alpha blending
	//Will be used to blend billboarded geometry with the background
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//Create the blend state
	renderer->CreateBlendState(&blendStateDesc, &blendState);

	// Setup the description of the dynamic matrix constant buffer that is in the geometry shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant matrix buffer pointer so we can access it in the geometry shader
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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
	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	//Create a buffer that will pass in user defined data into the geometry shader
	//Right now it only passes in the billboard size
	sizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	sizeBufferDesc.ByteWidth = sizeof(SizeBufferType);
	sizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sizeBufferDesc.MiscFlags = 0;
	sizeBufferDesc.StructureByteStride = 0;
	//Create the buffer
	renderer->CreateBuffer(&sizeBufferDesc, NULL, &sizeBuffer);
}

void GeometryShader::initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadGeometryShader(gsFilename);
}


void GeometryShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, int billSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	//Lock the buffer to the directx device, write the data to the buffer to pass them into the shaders, unlock it and finally pass the data into the specified shader
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview; //viewMatrix
	dataPtr->projection = tproj; //projectionMatrix
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional buffers
	//Pass billboard size data into the geometry shader
	SizeBufferType* sizePtr;
	deviceContext->Map(sizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	sizePtr = (SizeBufferType*)mappedResource.pData;
	sizePtr->size = billSize;
	deviceContext->Unmap(sizeBuffer, 0);
	deviceContext->GSSetConstantBuffers(1, 1, &sizeBuffer);

	//Send samplers and textures to the pixel shader
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	//Set the blend state in the output merger
	deviceContext->OMSetBlendState(blendState, 0, 0xffffffff);
}

void GeometryShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



