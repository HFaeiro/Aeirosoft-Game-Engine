#include "TextureShader.h"
#pragma comment(lib, "d3dcompiler.lib")
void TextureShader::init(ID3D11Device* pDevice, HWND)
{
	
	Microsoft::WRL::ComPtr < ID3D10Blob> pBlob;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	if (FAILED(D3DReadFileToBlob(L"texturev.cso", &pBlob)))
		throw;

	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);


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

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	if (FAILED(pDevice->CreateInputLayout(polygonLayout, numElements, pBlob->GetBufferPointer(),
															pBlob->GetBufferSize(), &pLayout)))
		throw;

	if (FAILED(D3DReadFileToBlob(L"texturep.cso", &pBlob)))
		throw;

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = static_cast<UINT>(sizeof(MatrixBufferType) + (16 - (sizeof(MatrixBufferType)) % 16 ));
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(pDevice->CreateBuffer(&matrixBufferDesc, NULL, &pMatrixBuffer)))
		throw;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	if (FAILED(pDevice->CreateSamplerState(&samplerDesc, &pSampleState)))
		throw;



}


TextureShader::TextureShader()
{
}

TextureShader::~TextureShader()
{
}

void TextureShader::render(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	SetShaderParameters(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix);


	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(pLayout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);
	pDeviceContext->PSSetSamplers(0, 1, pSampleState.GetAddressOf());


	return;

}



bool TextureShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//worldMatrix = worldMatrix* projectionMatrix;
	// Transpose the matrices to prepare them for the shader.
	worldMatrix		= DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix		= DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	if (FAILED(pDeviceContext->Map(pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	pDeviceContext->Unmap(pMatrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, pMatrixBuffer.GetAddressOf());

	//pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	return true;
}
