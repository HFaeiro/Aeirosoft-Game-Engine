#pragma once
#include "framework.h"
class IndexBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT size = 0;
public:
	ID3D11Buffer* Get() const { return buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
	UINT					Size() const { return this->size; }

	HRESULT Init(ID3D11Device* pDevice, ULONG* data, UINT numIndices)
	{
		this->size = numIndices;
		D3D11_BUFFER_DESC indexBuffDesc;
		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexBuffDesc, sizeof(D3D11_BUFFER_DESC));

		indexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBuffDesc.ByteWidth = sizeof(DWORD) * numIndices;
		indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBuffDesc.CPUAccessFlags = 0;
		indexBuffDesc.MiscFlags = 0;


		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = data;


		// Create the index buffer.
		return pDevice->CreateBuffer(&indexBuffDesc, &indexData, this->buffer.GetAddressOf());

	}


};

