#pragma once
#include "framework.h"
#include "inc\DirectX\SimpleMath.h"
#include <d3dcompiler.h>


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

public:
	TextureShader();
	~TextureShader();
	bool init(ID3D11Device*, HWND);
	void SetShaders(ID3D11DeviceContext* pDeviceContext);
	void Render(ID3D11DeviceContext*, DirectX::XMMATRIX world);

	void UpdateWorldMatrixBuffer(ID3D11DeviceContext* pDeviceContext, DirectX::XMMATRIX worldMatrix);

	void UpdateViewProjectionMatrixBuffer(ID3D11DeviceContext*, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

private:

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

	Microsoft::WRL::ComPtr < ID3D11VertexShader	> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr < ID3D11PixelShader	> pPixelShader = nullptr;
	Microsoft::WRL::ComPtr < ID3D11InputLayout	> pLayout = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pViewProjectionMatrixBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pWorldMatrixBuffer = nullptr;
	Microsoft::WRL::ComPtr < ID3D11SamplerState> pSampleState = nullptr;

};

