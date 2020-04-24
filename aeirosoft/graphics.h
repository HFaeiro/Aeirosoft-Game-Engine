#pragma once
#include "framework.h"
#include "helper.h"
#include <DirectXcolors.h>
#include "camera.h"
#include "TextureShader.h"
#include "Inc/DirectX/PrimitiveBatch.h"
#include "Inc/DirectX/VertexTypes.h"
#include "Inc/DirectX/SpriteBatch.h"
#include "Inc/DirectX/SpriteFont.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "texture.h"
#include "Inc/DirectX/Audio.h"
#pragma comment(lib,"DirectXTKAudioWin8.lib")
class graphics
{
public:

	graphics(const graphics& g);
	graphics(HWND hWnd, bool fullscreen, bool vSync);
	~graphics();
	const graphics& operator = (graphics& g) { g = *this; }

	

	std::unique_ptr<DirectX::SoundEffect> CreateSound(std::wstring name);

	bool Initialize();
	void CreateFloor(DirectX::XMFLOAT2 from, DirectX::XMFLOAT2 to,texture t, bool temporary = false);
	//void CreateWall(DirectX::XMFLOAT3 from, DirectX::XMFLOAT3 to, bool temporary);
	void CreateIndexAndVectorBuffers(std::vector<Vertex> vertices, DWORD indecies[], VertexBuffer& vb, IndexBuffer& ib);
	void UpdateOrthoProjectionMatrix();
	void ChangeFOV(float degrees);
	void BeginScene(float, float, float, float);
	//void Render();
	void EndScene();
	HWND GetWindow() { return m_Window; }
	void Resize();
	void AdjustCameraPos(camera::movementType type, float amount);
	void SetCamRotation(float x, float y, float z);
	//void DrawBoundingBoxes(model m);
	void SetFullScreen();
	void TurnZBufferOn();
	void TurnZBufferOff();
	Microsoft::WRL::ComPtr < ID3D11Device	> GetDevice();
	Microsoft::WRL::ComPtr < ID3D11DeviceContext	> GetDeviceContext();
	//void TurnOnAlphaBlending();
	//void TurnOffAlphaBlending();
	void TurnOnCulling();
	void TurnOffCulling();
	void SetDefaultViewport();
	camera m_Camera;


	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX Get2DViewMatrix();
	void Begin3DScene();
	void Begin2DScene();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();

	std::unique_ptr<DirectX::SpriteBatch> pSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> pSpriteFont;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

	TextureShader m_TextureShader;

protected:
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	bool m_vsync_enabled = false;
	//int m_videoCardMemory = 0;
	bool fullScreen = false;

	Microsoft::WRL::ComPtr < ID3D11Device			> pDevice = nullptr;
	Microsoft::WRL::ComPtr < IDXGISwapChain			> pSwap = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext	> pContext = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView > pTarget = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Texture2D		> pStencil = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilState> pStencilState = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilState> pDisabledStencilState = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilView	> pStencilView = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RasterizerState	> pRasterState = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RasterizerState	> pRasterStateNoCull = nullptr;
	Microsoft::WRL::ComPtr < ID3D11BlendState		> pBlendState = nullptr;
	Microsoft::WRL::ComPtr < ID3D11Resource			> pBackBuffer = nullptr;

	//std::unique_ptr			< DirectX::AudioEngine	> pAudio = nullptr;

	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_OrthoMatrix;
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_DefaultViewMatrix;
	HWND m_Window;

	float screenNear = .01f;
	float renderDistance = 15000.f;
	float FOV, aspectRatio, fovDegrees;

	float roty = 0.f;



	bool CreateDeviceAndSwap();
	bool InitializeAudioEngine();
	bool CreateRasterStates();
	bool CreateDepthStencil();

	bool CreateBlendState();
	




};

