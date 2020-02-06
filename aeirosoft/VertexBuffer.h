#pragma once
#include "framework.h"
#include "Vertex.h"

class VertexBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT size = 0;

public:

	VertexBuffer() {}
	VertexBuffer(const VertexBuffer& rhs)
	{
		this->buffer = rhs.buffer;
		this->size = rhs.size;
		this->stride = rhs.stride;
	}
	VertexBuffer & operator = (const VertexBuffer& rhs)
	{
		this->buffer = rhs.buffer;
		this->size = rhs.size;
		this->stride = rhs.stride;
		return *this;
	}

	ID3D11Buffer*			Get() const				{ return buffer.Get(); }
	ID3D11Buffer* const*	GetAddressOf() const	{ return buffer.GetAddressOf(); }
	UINT					Size() const			{ return this->size; }
	const UINT				GetStride() const		{ return this->stride; }
	const UINT *			GetStridePtr() const	{ return &this->stride; }

	
	HRESULT Init(ID3D11Device* pDevice, Vertex* data, UINT numVert)
	{
		this->size = numVert;
		this->stride = sizeof(Vertex);
		D3D11_BUFFER_DESC vertexBuffDesc;

		ZeroMemory(&vertexBuffDesc, sizeof(D3D11_BUFFER_DESC));
		
		// Set up the description of the static vertex buffer.
		vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBuffDesc.ByteWidth = sizeof(Vertex) * numVert;
		vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBuffDesc.CPUAccessFlags = 0;
		vertexBuffDesc.MiscFlags = 0;
		vertexBuffDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = data;


		// Now create the vertex buffer.

		return pDevice->CreateBuffer(&vertexBuffDesc, &vertexData, &buffer);

	}

};


