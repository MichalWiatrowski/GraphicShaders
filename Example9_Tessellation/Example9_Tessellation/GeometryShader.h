//Shader for creating billboards
//The shader will receive a point mesh, pass in a texture and the size of the billboard
//and create it in the geometry shader
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class GeometryShader : public BaseShader
{
private:
	//Buffer definitions that will be used in hlsl shaders
	struct SizeBufferType
	{
		float size;
		XMFLOAT3 padding;
	};
public:

	GeometryShader(ID3D11Device* device, HWND hwnd);
	~GeometryShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, int billSize);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);
private:
	//Create buffers and sampler states
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* sizeBuffer;
	//Create a blend state 
	ID3D11BlendState* blendState;
};
