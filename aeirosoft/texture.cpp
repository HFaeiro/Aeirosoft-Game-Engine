#include "texture.h"

texture::texture()
{

}
texture::texture(ID3D11Device* pDevice, const color& color, aiTextureType type)
{
	this->Initialize1x1colorTexture(pDevice, color, type);
}
texture::texture(ID3D11Device* pDevice, const color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->InitializecolorTexture(pDevice, colorData, width, height, type);
}
texture::texture(ID3D11Device* pDevice, std::wstring filepath, aiTextureType type)
{

	HRESULT hr;
	this->type = type;
	if (helper::strings::GetFileExtention(filepath) == L".dds")
	{
		hr = DirectX::CreateDDSTextureFromFile(pDevice, filepath.c_str(), &pTexture, &pTextureView);
		if(FAILED(hr))
			Initialize1x1colorTexture(pDevice, colors::UnloadedTexturecolor, type);
		return;

	}
	else
	{
		
		hr = DirectX::CreateWICTextureFromFile(pDevice, filepath.c_str(), pTexture.GetAddressOf(), pTextureView.GetAddressOf());
		if (FAILED(hr))
			Initialize1x1colorTexture(pDevice, colors::UnloadedTexturecolor, type);

		return;

	}



}
texture::~texture()
{

}

aiTextureType texture::GetType()
{
	return this->type;
}

ID3D11ShaderResourceView* texture::GetTextureResourceView()
{
	return this->pTextureView.Get();
}

ID3D11ShaderResourceView** texture::GetTextureResourceViewAddr()
{
	return this->pTextureView.GetAddressOf();
}



ID3D11Resource* texture::GetTexture()
{
	return pTexture.Get();
}

void texture::Initialize1x1colorTexture(ID3D11Device* pDevice, const color& colorData, aiTextureType type)
{
	InitializecolorTexture(pDevice, &colorData, 1, 1, type);
}

void texture::InitializecolorTexture(ID3D11Device* pDevice, const color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2dTexture = nullptr;
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = colorData;
	subData.SysMemPitch = width * sizeof(color);

	if (FAILED(pDevice->CreateTexture2D(&textDesc, &subData, &p2dTexture)))
		return;
	pTexture = p2dTexture;
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textDesc.Format);
	if (FAILED(pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView)))
		return;

}
