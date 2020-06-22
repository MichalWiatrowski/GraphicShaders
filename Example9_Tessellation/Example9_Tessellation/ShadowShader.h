//ShadowShader.h
//Main shader
//Shader that will draw geometry, apply lightning and shadows
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "DXF.h"

using namespace std;
using namespace DirectX;

class ShadowShader : public BaseShader
{
private:
	//Buffer definitions that will be used in hlsl shaders
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[3];
		XMMATRIX lightProjection[3];
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[3];
		XMFLOAT4 diffuse[3];
		XMFLOAT4 direction[3];
		XMFLOAT4 spotlightPosition;
		
	};
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};
public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, ID3D11ShaderResourceView*depthMap3, Light* light[], XMFLOAT3 cameraPosition);

private:
	void initShader(WCHAR*, WCHAR*);
private:
	//Create buffers and sampler states
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
};

#endif