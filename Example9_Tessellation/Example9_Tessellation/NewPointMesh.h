#pragma once
//Point mesh, derived from the BaseMesh class created by Paul Robertson
//Generates an amount of D3D11_PRIMITIVE_TOPOLOGY_POINTLIST points
//The points created have normals but no texture coordinates
//points_Amount is the number of points created in this mesh, spaced out by two units on the positive x axis
#include "DXF.h"

using namespace DirectX;

class NewPointMesh : public BaseMesh
{
public:

	NewPointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int points_Amount = 1);
	~NewPointMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
protected:
	int pointsAmount;
	void initBuffers(ID3D11Device* device);
};
