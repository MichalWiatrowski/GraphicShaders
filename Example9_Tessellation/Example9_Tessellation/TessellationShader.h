//TessellationShader.h
//Shader that will tessellate plane meshes and apply vertex manipulation in the geometry shader
//Height map and sin wave example
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class TessellationShader : public BaseShader
{
private:
	//Buffer definitions that will be used in hlsl shaders
	struct TessellationBufferType
	{
		XMFLOAT3 cameraPos;
		float planeID;
		float minMaxTessFactor;
		XMFLOAT3 padding;
		float hardDistance;
		XMFLOAT3 padding2;
	};
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};
	struct SinWaveType
	{
		XMFLOAT3 time_speed_height;
		float padding;
	};
public:

	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* heightMapTexture, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPos, float planeID, float minMaxTessFactor, float hardDistance, Light* light, float deltaTime, float speed, float height);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	//Create buffers and sampler states
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* tessellationBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* sinWaveBuffer;
	//Create a blend state
	ID3D11BlendState* blendState;
};
