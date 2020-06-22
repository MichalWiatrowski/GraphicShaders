//Simple depth shader
//Used for creating shadows
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class DepthShader : public BaseShader
{
public:
	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	//Create a matrix buffer
	ID3D11Buffer* matrixBuffer;

};
