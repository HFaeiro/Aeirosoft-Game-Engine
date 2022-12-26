#include "graphics.h"

graphics::graphics(const graphics& g)
{
	if (this != &g)
	{

		this->pContext = g.pContext;
		this->pDevice = g.pDevice;
		this->pSwap = g.pSwap;
		this->m_TextureShader = g.m_TextureShader;
		this->m_ViewMatrix = g.m_ViewMatrix;
		this->m_OrthoMatrix = g.m_OrthoMatrix;
		this->m_WorldMatrix = g.m_WorldMatrix;
		this->m_ProjectionMatrix = g.m_ProjectionMatrix;
		this->m_Camera = g.m_Camera;
		this->pDisabledStencilState = g.pDisabledStencilState;
		this->m_Window = g.m_Window;
		
	}

}
graphics::graphics(HWND hWnd, bool fullscreen, bool vSync)
{
	this->m_Window = hWnd;
	this->m_vsync_enabled = vSync;
	this->fullScreen = fullscreen;
}
graphics::~graphics()
{

	pSwap->SetFullscreenState(false, NULL);

}


bool graphics::CreateDeviceAndSwap()
{

	RECT r = helper::window::GetRect(m_Window);

	int height = r.bottom;
	int width = r.right;

	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_Window;
	sd.Windowed = !fullScreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, nullptr, 0, D3D11_SDK_VERSION, &sd,
		&pSwap, &pDevice, nullptr,
		&pContext);

	if (FAILED(hr))
		return false;

	return true;
}
bool graphics::InitializeAudioEngine()
{
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return false;
	

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif

	audEngine = std::make_unique<DirectX::AudioEngine>(eflags);
	return true;
}
std::unique_ptr<DirectX::SoundEffect> graphics::CreateSound(std::wstring name)
{
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	return std::make_unique<DirectX::SoundEffect>(audEngine.get(), name.c_str());
}
bool graphics::Initialize()
{

	this->fovDegrees = 100.f;
	FOV = (fovDegrees / 360) * DirectX::XM_PI;



	HRESULT hr;

	if (!CreateDeviceAndSwap())
		return false;
	
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	
	if (FAILED(hr))
		return false;
	
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	pBackBuffer->Release();

	if (FAILED(hr))
		return false;

	if(!CreateDepthStencil())
		return false;

	TurnZBufferOn();

	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pStencilView.Get());

	if (!CreateRasterStates())
		return false;

	TurnOnCulling();

	SetDefaultViewport();

	if (!CreateBlendState())
		return false;

	if (!m_TextureShader.init(pDevice.Get(),m_Window))
		return false;

	UpdateOrthoProjectionMatrix();

	// Initialize the world matrix to the identity matrix.
	m_WorldMatrix = DirectX::XMMatrixIdentity();



	m_Camera.setPosition(0.f, 0.f, -.1f);
	m_DefaultViewMatrix = m_Camera.getViewMatrix();
	m_Camera.setProjection(m_ProjectionMatrix);

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(pContext.Get());
	
	pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(this->pContext.Get());
	pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice.Get(), L"Data\\Fonts\\hFontSm.spritefont");
//		m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), wssAcc.str().c_str(), DirectX::XMFLOAT2(0, 40));

	InitializeAudioEngine();
	return true;

}

void graphics::CreateIndexAndVectorBuffers(std::vector<Vertex> vertices, DWORD indecies[], VertexBuffer& vb, IndexBuffer& ib)
{
	ib.Init(this->pDevice.Get(), indecies, 6);
	vb.Init(pDevice.Get(), vertices.data(), vertices.size() + 1);
}

void graphics::UpdateOrthoProjectionMatrix()
{
	RECT r = helper::window::GetRect(m_Window);
	int height = r.bottom;
	int width = r.right;
	aspectRatio = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, screenNear, renderDistance);

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = DirectX::XMMatrixOrthographicLH((float)width, (float)height, screenNear, renderDistance);

}
void graphics::ChangeFOV(float degrees)
{
	FOV = (degrees / 360) * DirectX::XM_PI;
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, screenNear, renderDistance);
	m_Camera.setProjection(m_ProjectionMatrix);
	fovDegrees = degrees;
}
float graphics::GetFovDegrees()
{
	return fovDegrees;
}
void graphics::BeginScene(float red, float green, float blue, float alpha)
{
	
	const float color[] = { red, green, blue, alpha };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	pContext->RSSetState(pRasterState.Get());
	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pStencilView.Get());
	TurnZBufferOn();
	m_TextureShader.SetShaders(pContext.Get());
//




	m_ViewMatrix = m_Camera.getViewMatrix();

	m_TextureShader.Render(pContext.Get(),m_WorldMatrix);

	pContext->OMSetBlendState(pBlendState.Get(), 0, 0xffffffff);

	if (!audEngine->Update())
	{
		if (audEngine->IsCriticalError())
		{
			int i = 0;
			i++;
		}
	}
}



void graphics::EndScene()
{
	m_Camera.render(false);
	if(m_vsync_enabled)
 		pSwap->Present(1u, 0u);
	else
		pSwap->Present(0u, 0u);

	
}

bool graphics::CreateDepthStencil()
{

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	RECT r = helper::window::GetRect(m_Window);

	HRESULT hr;

	depthBufferDesc.Width = r.right;
	depthBufferDesc.Height = r.bottom;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pStencil);

	if (FAILED(hr))
		return false;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pStencilState);

	if (FAILED(hr))
		return false;

	depthStencilDesc.DepthEnable = false;
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDisabledStencilState);

	if (FAILED(hr))
		return false;

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = pDevice->CreateDepthStencilView(pStencil.Get(), &depthStencilViewDesc, &pStencilView);

	if (FAILED(hr))
		return false;

	return true;

}


bool graphics::CreateBlendState()
{
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = FALSE;
	blendStateDesc.IndependentBlendEnable = FALSE;
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(pDevice->CreateBlendState(&blendStateDesc, &pBlendState))) {
		return false;
	}

	return true;
}


bool graphics::CreateRasterStates()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	if (FAILED(pDevice->CreateRasterizerState(&rasterDesc, &pRasterState)))
		return false;

	rasterDesc.CullMode = D3D11_CULL_NONE;

	if (FAILED(pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateNoCull)))
		return false;



	return true;
}

void graphics::TurnOnCulling()
{
	pContext->RSSetState(pRasterState.Get());
}

void graphics::TurnOffCulling()
{
	pContext->RSSetState(pRasterStateNoCull.Get());
}

void graphics::SetDefaultViewport()
{
	D3D11_VIEWPORT viewport;

	RECT r = helper::window::GetRect(m_Window);
	// Setup the viewport for rendering.
	viewport.Width = (float)r.right;
	viewport.Height = (float)r.bottom;
	viewport.MinDepth = 0.00f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	pContext->RSSetViewports(1, &viewport);
}

void graphics::SetFullScreen()
{
	
	this->fullScreen = !this->fullScreen;

	pSwap->SetFullscreenState(this->fullScreen, NULL);

}

void graphics::TurnZBufferOn()
{
	pContext->OMSetDepthStencilState(pStencilState.Get(), 1);
}

void graphics::TurnZBufferOff()
{
	pContext->OMSetDepthStencilState(pDisabledStencilState.Get(), 1);
}

Microsoft::WRL::ComPtr<ID3D11Device> graphics::GetDevice()
{
	if (this == nullptr)
		return nullptr;
	return pDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> graphics::GetDeviceContext()
{ 
	if (this == nullptr)
		return nullptr;
	return pContext;
}

DirectX::XMMATRIX graphics::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

DirectX::XMMATRIX graphics::GetWorldMatrix()
{
	return m_WorldMatrix;
}

DirectX::XMMATRIX graphics::GetOrthoMatrix()
{
	return m_OrthoMatrix;
}


DirectX::XMMATRIX graphics::GetViewMatrix()
{
	return m_ViewMatrix;
}
DirectX::XMMATRIX graphics::Get2DViewMatrix()
{
	return m_DefaultViewMatrix;
}
void graphics::Begin3DScene()
{
	
	m_TextureShader.UpdateViewProjectionMatrixBuffer(pContext.Get(), m_ViewMatrix, m_ProjectionMatrix);
	//pContext->RSSetState(pRasterState.Get());
	//pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pStencilView.Get());
	TurnZBufferOn();
	m_TextureShader.SetShaders(pContext.Get());

}
void graphics::Begin2DScene()
{
	TurnZBufferOff();
	m_TextureShader.UpdateViewProjectionMatrixBuffer(pContext.Get(), m_DefaultViewMatrix, m_OrthoMatrix);
	m_TextureShader.Render(pContext.Get(), DirectX::XMMatrixIdentity());
	m_TextureShader.SetShaders(pContext.Get());
}

void graphics::Resize()
{

	RECT r = helper::window::GetRect(m_Window);
	if (r.right == 0)
	{
		int err = GetLastError();
		return;
	}
	



	pSwap->ResizeBuffers(1, r.right, r.bottom, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_EFFECT_DISCARD);

	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	pBackBuffer->Release();

	CreateDepthStencil();
	
//	SetDefaultViewport();
	
	UpdateOrthoProjectionMatrix();

}

void graphics::AdjustCameraPos(camera::movementType type, float amount)
{
	m_Camera.adjustPosition(type, amount);
}

void graphics::SetCamRotation(float x, float y, float z)
{
	m_Camera.setRotation(x, y, z);
}



