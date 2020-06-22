//Simple depth shader
//Altered version of previous DepthShader
//Since we are 'exploding objects', we need to explode the shadows as well
//To do that we need to run the same code for it
//Shaders very similar, only differences is the data being passed and where it is calculated (positions and passed into the geometry shader and calculated there instead of the vertex shader)
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class DepthShaderGeo : public BaseShader
{
	//Buffer definitions that will be used in hlsl shaders
	struct TimeBufferType
	{
		float time;
		XMFLOAT3 padding;
	};
public:

	DepthShaderGeo(ID3D11Device* device, HWND hwnd);
	~DepthShaderGeo();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, float operationID, float deltaTime);

private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR*, WCHAR*, WCHAR*);

private:
	//Create buffers
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* timeBuffer;
};
