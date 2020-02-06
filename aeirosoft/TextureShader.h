#pragma once
#include "framework.h"
#include <d3dcompiler.h>
#include <fstream>
class TextureShader
{
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShader();
	~TextureShader();
	void init(ID3D11Device*, HWND);
	void render(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

private:

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

	Microsoft::WRL::ComPtr < ID3D11VertexShader	> pVertexShader	= nullptr;
	Microsoft::WRL::ComPtr < ID3D11PixelShader	> pPixelShader	= nullptr;
	Microsoft::WRL::ComPtr < ID3D11InputLayout	> pLayout		= nullptr;
	Microsoft::WRL::ComPtr < ID3D11Buffer		> pMatrixBuffer	= nullptr;
	Microsoft::WRL::ComPtr < ID3D11SamplerState> pSampleState	= nullptr;

};

