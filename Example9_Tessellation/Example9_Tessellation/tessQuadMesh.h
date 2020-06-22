#pragma once
//Simple quad mesh with 4 sides, texture coordinates and normals to demonstrate distance based tessellation 
#include "DXF.h"

using namespace DirectX;

class tessQuadMesh : public BaseMesh
{
public:
	
	tessQuadMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~tessQuadMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
protected:
	
	void initBuffers(ID3D11Device* device);
};