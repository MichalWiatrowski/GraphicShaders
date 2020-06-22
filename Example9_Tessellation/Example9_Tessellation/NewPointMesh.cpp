#include "NewPointMesh.h"
//Point Mesh

//Get the points amount and init buffers
NewPointMesh::NewPointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int points_Amount)
{
	pointsAmount = points_Amount;
	initBuffers(device);
}

//Release objects and resources
NewPointMesh::~NewPointMesh()
{
	//Run parent class deconstructor
	BaseMesh::~BaseMesh();
}


// Build shape and fill buffers.
void NewPointMesh::initBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Set the vertex and index amount, in this case the amount of points
	vertexCount = pointsAmount;
	indexCount = vertexCount;

	//Create vertex and index arrays
	VertexType* vertices = new VertexType[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	for (int i = 0; i < pointsAmount; i++) //Create x amount of  D3D11_PRIMITIVE_TOPOLOGY_POINTLIST points
	{
		vertices[i].position = XMFLOAT3(0.0f + (i * 3), 0.0f, 0.0f); // Creates points with 2 units in between
		vertices[i].normal = XMFLOAT3(0.0f, 0.0f, -1.0f); //Create normals towards towards the screen

		indices[i] = i; //Update the indices array
	}

	//Create buffer descriptions and fill the buffers
	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}
// Send Geometry data to the GPU
void NewPointMesh::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}


