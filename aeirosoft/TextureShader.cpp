#include "TextureShader.h"
#ifdef _DEBUG
#pragma comment(lib, "d3dcompiler.lib")
#endif // DEBUG
#include <fstream>


TextureShader::TextureShader()
{
}

TextureShader::~TextureShader()
{
}

bool TextureShader::init(ID3D11Device* pDevice, HWND)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr < ID3D10Blob> pBlob;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3] = {};
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
#ifdef _DEBUG



	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (FAILED(hr))
		return false;
	hr = D3DWriteBlobToFile(pBlob.Get(), L"vs", true);
	if (FAILED(hr))
		return false;
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
#else
	std::ifstream ifs(L"vs", std::ios::in | std::ios::binary);
	if (!ifs.is_open())
	{

		MessageBox(NULL, L"Failed To find file 'vs' please make sure its with this file", L"Error Can't load vertex shaders!", MB_OK);
		return false;

	}
	ifs.seekg(0, std::ios::end);
	size_t size = ifs.tellg();
	
	char* buffer = new char[size];
	ifs.seekg(0, std::ios::beg);
	ifs.read(buffer, size);
	ifs.close();
	hr = pDevice->CreateVertexShader(buffer, size, nullptr, &pVertexShader);
#endif // DEBUG	
	
	if (FAILED(hr))
		return false;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
#ifdef _DEBUG

	hr = pDevice->CreateInputLayout(polygonLayout, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout);
	if (FAILED(hr))
		return false;

	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (FAILED(hr))
		return false;
	hr = D3DWriteBlobToFile(pBlob.Get(), L"ps", true);
	if (FAILED(hr))
		return false;
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
#else

	hr = pDevice->CreateInputLayout(polygonLayout, numElements, buffer, size, &pLayout);
	if (FAILED(hr))
		return false;


	ifs.open(L"ps", std::ios::in | std::ios::binary);
	if (!ifs.is_open())
	{

		MessageBox(NULL, L"Failed To find file 'ps' please make sure its with this file", L"Error Can't load pixel shaders!", MB_OK);
			return false;

	}
	ifs.seekg(0, std::ios::end);
	size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	ifs.read(buffer, size);
	ifs.close();
	hr = pDevice->CreatePixelShader(buffer, size, nullptr, &pPixelShader);
	free(buffer);
#endif // DEBUG	
	if (FAILED(hr))
		return false;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(WorldMatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &pWorldMatrixBuffer);
	if (FAILED(hr))
		return false;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.

	matrixBufferDesc.ByteWidth = sizeof(ViewProjectionBuffer);


	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &pViewProjectionMatrixBuffer);
	if (FAILED(hr))
		return false;

	matrixBufferDesc.ByteWidth = sizeof(LightBufferType);
	hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &pLightBuffer);
	if (FAILED(hr))
		return false;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = pDevice->CreateSamplerState(&samplerDesc, &pSampleState);
	if (FAILED(hr))
		return false;

	return true;
}
void TextureShader::SetShaders(ID3D11DeviceContext* pDeviceContext)
{
	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(pLayout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);
	pDeviceContext->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
}



void TextureShader::Render(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix)
{
	UpdateWorldMatrixBuffer(pDeviceContext, worldMatrix);

}

void TextureShader::UpdateWorldMatrixBuffer(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WorldMatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	HRESULT hr;

	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);


	if (FAILED(pDeviceContext->Map(pWorldMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
				return ;

	dataPtr = (WorldMatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldMatrix;

	pDeviceContext->Unmap(pWorldMatrixBuffer.Get(), 0);

	//pDeviceContext->UpdateSubresource(pWorldMatrixBuffer.GetAddressOf(), 0, nullptr, &worldMatrix, 0, 0);

	pDeviceContext->VSSetConstantBuffers(0, 1, pWorldMatrixBuffer.GetAddressOf());

}

void TextureShader::UpdateViewProjectionMatrixBuffer(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ViewProjectionBuffer* dataPtr;
	LightBufferType* dataPtr2;
	unsigned int bufferNumber;

	//worldMatrix = worldMatrix* projectionMatrix;
	// Transpose the matrices to prepare them for the shader.
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	if (FAILED(pDeviceContext->Map(pViewProjectionMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return;

	dataPtr = (ViewProjectionBuffer*)mappedResource.pData;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	pDeviceContext->Unmap(pViewProjectionMatrixBuffer.Get(), 0);

	pDeviceContext->VSSetConstantBuffers(1, 1, pViewProjectionMatrixBuffer.GetAddressOf());

	if (FAILED(pDeviceContext->Map(pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return;

	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->ambientColor = { .6f, .6f, .6f , 1.f };
	dataPtr2->diffuseColor = { 0.2f, 0.2f, 0.1f, .1f };
	dataPtr2->lightDirection = { 0.0f, 1.0f, 1.0f, 1.f};


	pDeviceContext->Unmap(pLightBuffer.Get(), 0);




	pDeviceContext->PSSetConstantBuffers(0, 1, pLightBuffer.GetAddressOf());

}


