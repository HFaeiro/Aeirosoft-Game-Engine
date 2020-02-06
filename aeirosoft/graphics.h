#pragma once
#include "framework.h"

#include "Inc/DirectX/SpriteBatch.h"
#include "Inc/DirectX/SpriteFont.h"
#include <wrl.h>
#include "model.h"
#include "camera.h"
#include "TextureShader.h"
#include <memory>
#include "Timer.h"

#pragma comment (lib, "DirectXTK.lib")


class graphics
{
public:
	
	graphics(HWND);
	~graphics();
	const graphics& operator = (graphics& g) { g = *this; }

	void endFrame();
	void BeginScene(float red, float green, float blue, float alpha);
	void SetCamera(float, float, float);
	void AdjustCameraPos( camera::movementType, float);
	void SetCameraRotation(float, float, float);
	DirectX::XMMATRIX getViewMatrix();
	DirectX::XMMATRIX getProjectionMatrix();
	DirectX::XMMATRIX getWorldMatrix();

	std::vector<std::vector<Vertex>> getModelBounds() { return pModel.GetBounds(); }

	model pModel;
	camera pCamera;
	RECT GetRect(HWND hWnd);
private:
	
	Microsoft::WRL::ComPtr < ID3D11Device			> pDevice = nullptr;
	Microsoft::WRL::ComPtr < IDXGISwapChain			> pSwap = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext	> pContext = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView > pTarget = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Texture2D		> pStencil = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilState> pStencilState = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilView	> pStencilView = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RasterizerState	> pRasterState = nullptr;
	
	DirectX::XMMATRIX pprojectionMatrix;
	DirectX::XMMATRIX pworldMatrix;
	DirectX::XMMATRIX porthoMatrix;
	DirectX::XMMATRIX viewMatrix;
	HWND window;
	
	void getWindowDimentions();

	float screenNear = .01f;
	float screenDepth = 1000.0f;
	UINT height;
	UINT width;
	float fieldOfView, screenAspect;

	bool create2DTexture();
	bool setRasterState();
	void setViewport();

	TextureShader pTextureShader;
	std::unique_ptr<DirectX::SpriteBatch> pSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> pSpriteFont;

	Timer fpsTimer;

};

