//ShadowsExplosion.h
//Shader that will use the geometry shader to apply explosion effect, apply cook-torrance lightining and shadows
//Shader very similar to the core ShadowShader.h but has few changes; 
//has its own vertex shader to pass the data correctly to the geometry shader
//and in the geometry shader explode the shape along the normal vector
#ifndef _ShadowsExplosion_H_
#define _ShadowsExplosion_H_

#include "DXF.h"

using namespace std;
using namespace DirectX;


class ShadowsExplosion : public BaseShader
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
		float time;
	};
public:

	ShadowsExplosion(ID3D11Device* device, HWND hwnd);
	~ShadowsExplosion();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, ID3D11ShaderResourceView*depthMap3, Light* light[], XMFLOAT3 cameraPosition, float deltaTime);

private:
	void initShader(WCHAR*, WCHAR*, WCHAR*);
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);

private:
	//Create buffers and sampler states
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
};

#endif