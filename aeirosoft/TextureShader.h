#pragma once
#include "framework.h"
#include "inc\DirectX\SimpleMath.h"
#ifdef _DEBUG
	#include <d3dcompiler.h>
#endif // DEBUG




class TextureShader
{
	struct WorldMatrixBufferType
	{
		DirectX::XMMATRIX world;
	};
	struct ViewProjectionBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	struct LightBufferType
	{
		DirectX::XMVECTOR ambientColor;
		DirectX::XMVECTOR diffuseColor;
		DirectX::XMVECTOR lightDirection;
	};
	struct BoneBuffer
	{
		BoneBuffer()
		{
			memset(boneTransforms, 0, sizeof(DirectX::XMMATRIX) * ARRAYSIZE(boneTransforms));
		}
		DirectX::XMMATRIX boneTransforms[50];
	};

public:
	TextureShader();
	~TextureShader();
	bool init(ID3D11Device*, HWND);
	void SetShaders(ID3D11DeviceContext* pDeviceContext, bool Skinned = false);
	void Render(ID3D11DeviceContext*, DirectX::XMMATRIX world);

	void UpdateWorldMatrixBuffer(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix);

	void UpdateBonesBuffer(ID3D11DeviceContext* pDeviceContext, std::vector<DirectX::XMMATRIX> transforms);

	void UpdateViewProjectionMatrixBuffer(ID3D11DeviceContext*, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

private:


	Microsoft::WRL::ComPtr < ID3D11VertexShader	> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr < ID3D11VertexShader	> pSkinnedVertexShader = nullptr;
	Microsoft::WRL::ComPtr < ID3D11PixelShader	> pPixelShader = nullptr;
	Microsoft::WRL::ComPtr < ID3D11InputLayout	> pLayout = nullptr;
	Microsoft::WRL::ComPtr < ID3D11InputLayout	> pSkinnedLayout = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pViewProjectionMatrixBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pWorldMatrixBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pLightBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pBoneBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11SamplerState	> pSampleState = nullptr;

};

