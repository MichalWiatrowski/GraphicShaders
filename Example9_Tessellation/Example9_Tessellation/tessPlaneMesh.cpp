#include "tessPlaneMesh.h"
// tessPlaneMesh.cpp
// Simple plane mesh created of quads with a topology of  D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST for tessellation

tessPlaneMesh::tessPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int quadCount)
{
	quadsAmount = quadCount;
	initBuffers(device);
}
// Release resources.
tessPlaneMesh::~tessPlaneMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}


// Generates plane and fill buffers
void tessPlaneMesh::initBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Calculate the amount of vertices in the plane, 4 (since quads) * amount of quads in the x direction * amount of quads in the z direction
	vertexCount = 4 * quadsAmount * quadsAmount;
	indexCount = vertexCount;

	//Create vertex and index arrays
	VertexType* vertices = new VertexType[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	int i = 0, j = 0, verticeCount = 0;
	float u = 0, v = 0, increment = 1.0f / quadsAmount;

	//Generate quads, with texture coordinates and normals (postive in the y direction
	for (i = 0; i < quadsAmount ; i++)
	{
		for (j = 0; j < quadsAmount ; j++)
		{
			vertices[verticeCount].position = XMFLOAT3(0.0f + i, 0.0f, 1.0f + j);  // Top left
			vertices[verticeCount].texture = XMFLOAT2(u + increment, v);
			vertices[verticeCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			indices[verticeCount] = verticeCount;
			verticeCount++; //increment vertice index

			vertices[verticeCount].position = XMFLOAT3(0.0f + i, 0.0f, 0.0f + j);  // bottom left.
			vertices[verticeCount].texture = XMFLOAT2(u, v);
			vertices[verticeCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			indices[verticeCount] = verticeCount;
			verticeCount++;

			vertices[verticeCount].position = XMFLOAT3(1.0f + i, 0.0f, 0.0f + j);  // bottom right.
			
			vertices[verticeCount].texture = XMFLOAT2(u, v + increment);
			vertices[verticeCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			indices[verticeCount] = verticeCount;
			verticeCount++;

			vertices[verticeCount].position = XMFLOAT3(1.0f + i, 0.0f, 1.0f + j);  // top right.
			vertices[verticeCount].texture = XMFLOAT2(u + increment, v + increment);
			vertices[verticeCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			indices[verticeCount] = verticeCount;
			verticeCount++;
			u += increment;
		}
		u = 0;
		v += increment;
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
void tessPlaneMesh::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
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


