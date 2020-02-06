#pragma once
#include "framework.h"
#include "Color.h"
#include <Assimp/material.h>

//#include "inc/DirectX/DDSTextureLoader.h"
//#pragma comment (lib, "DirectXTK.lib")


enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};


class texture
{
public:
	
	texture(ID3D11Device* pDevice, const Color& color, aiTextureType type);
	texture(ID3D11Device* pDevice, const Color* colorData, UINT width, UINT height, aiTextureType);
	~texture();
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddr();
	

	ID3D11Resource* GetTexture();

private:

	Microsoft::WRL::ComPtr< ID3D11Resource > pTexture;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > pTextureView;
	void Initialize1x1ColorTexture(ID3D11Device* pDevice, const Color& colorData, aiTextureType type);
	void InitializeColorTexture(ID3D11Device* pDevice, const Color* colorData, UINT width, UINT height, aiTextureType type);
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

};

