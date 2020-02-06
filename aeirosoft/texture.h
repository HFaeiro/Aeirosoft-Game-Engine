#pragma once
#include "framework.h"
#include "color.h"
#include <Assimp/material.h>
#include "helper.h"
#include "Inc/DirectX/DDSTextureLoader.h"
#include "Inc/DirectX/WICTextureLoader.h"

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};


class texture
{
public:

	texture();

	texture(ID3D11Device* pDevice, const color& color, aiTextureType type);
	texture(ID3D11Device* pDevice, const color* colorData, UINT width, UINT height, aiTextureType);
	texture(ID3D11Device* pDevice, std::wstring filepath, aiTextureType);
	~texture();
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddr();


	ID3D11Resource* GetTexture();

private:

	Microsoft::WRL::ComPtr< ID3D11Resource > pTexture;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > pTextureView;
	void Initialize1x1colorTexture(ID3D11Device* pDevice, const color& colorData, aiTextureType type);
	void InitializecolorTexture(ID3D11Device* pDevice, const color* colorData, UINT width, UINT height, aiTextureType type);
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

};

