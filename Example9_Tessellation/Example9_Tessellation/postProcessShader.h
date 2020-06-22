#pragma once
//Shader for post processing
//This shader class will be used for rendering the altered texture to an ortho mesh 
//to apply sfx post processing to the scene
#include "DXF.h"

using namespace std;
using namespace DirectX;

class postProcessShader : public BaseShader
{
private:
	//Buffer definitions that will be used in hlsl shaders
	struct DirectionalBlurBufferType
	{
		XMFLOAT2 direction;
		float distance;
		float samples;
	};
	struct ScreenSizeBufferType 
	{
		XMFLOAT2 screen_size;
		XMFLOAT2 mousePos;
		float postProcessType;
		float horizontal;
		XMFLOAT2 padding;
	};
public:
	postProcessShader(ID3D11Device* device, HWND hwnd);
	~postProcessShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, XMFLOAT2 screenSize, XMFLOAT2 mousePos, float postProcess, XMFLOAT2 direction, float distance, float horizontal);
private:
	void initShader(WCHAR*, WCHAR*);
private:
	//Create buffers and sampler states
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
	ID3D11Buffer* directionalBuffer;
};

