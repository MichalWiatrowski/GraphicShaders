// depth shader.cpp
#include "DepthShaderGeo.h"

//init shader and load hlsl shaders
DepthShaderGeo::DepthShaderGeo(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"depth_vs_test.cso", L"explosionShadows_gs.cso", L"depth_ps.cso");
}

//Release buffers and states
DepthShaderGeo::~DepthShaderGeo()
{
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
	
	if (timeBuffer)
	{
		timeBuffer->Release();
		timeBuffer = 0;
	}
	//Release base shader components
	BaseShader::~BaseShader();
}


void DepthShaderGeo::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	//Create buffer descriptions
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC timeBufferDesc;

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

	// Setup the description of the constant buffer that will pass offset data to the geometry shader
	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;
	//Create the buffer
	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer);
}

//init shader overload, loads in an extra shader (geometry shader)
void DepthShaderGeo::initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
	initShader(vsFilename, psFilename);
	loadGeometryShader(gsFilename);
}


void DepthShaderGeo::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, float operationID, float deltaTime)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);

	//Create a pointer to the buffer
	TimeBufferType* timePtr;
	// Lock the constant buffer so it can be written to, pass the data and unlock it
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	timePtr->time = deltaTime;
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->GSSetConstantBuffers(1, 1, &timeBuffer);
}


