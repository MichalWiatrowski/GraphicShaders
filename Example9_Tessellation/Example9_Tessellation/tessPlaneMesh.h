#pragma once
//Simple plane mesh, derived from BaseMesh
//This class has been taken from an older framework created by Paul Robertson and adapted for tessellation purposes
#include "DXF.h"

using namespace DirectX;

class tessPlaneMesh : public BaseMesh
{
public:

	tessPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int quadCount = 2);
	~tessPlaneMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
protected:
	int quadsAmount;
	void initBuffers(ID3D11Device* device);
};
