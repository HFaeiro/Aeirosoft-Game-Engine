#include "graphics.h"



graphics::graphics(HWND hWnd)
{
	fpsTimer.Start();
	window = hWnd;

	getWindowDimentions();


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
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	Microsoft::WRL::ComPtr < ID3D11Resource> pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	pBackBuffer->Release();

	if (!create2DTexture())
		throw;


	// Bind the render target view and depth stencil buffer to the output render pipeline.
	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pStencilView.Get());

	if (!setRasterState())
		throw;

	setViewport();

	// Setup the projection matrix.
	float fovDegrees = 90.f;
	fieldOfView = (fovDegrees / 360) * DirectX::XM_PI;
	screenAspect = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	pprojectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	
	// Initialize the world matrix to the identity matrix.
	pworldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	porthoMatrix = DirectX::XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);

	pModel.init("Data\\Objects\\nanosuit\\nanosuit.obj",pDevice.Get(),pContext.Get());

	pCamera.setPosition(0.f, 0.f, -20.f);
	pCamera.setProjection(pprojectionMatrix);
	pTextureShader.init(pDevice.Get(), hWnd);

	pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(this->pContext.Get());
	pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice.Get(), L"Data\\Fonts\\hFontSm.spritefont");



}

graphics::~graphics()
{
	
}

void graphics::BeginScene(float red, float green, float blue, float alpha)
{
	const float color[] = { red, green, blue, alpha };
	pContext->ClearRenderTargetView(pTarget.Get(), DirectX::Colors::Black);

	pContext->ClearDepthStencilView(pStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->RSSetState(pRasterState.Get());
	pContext->OMSetDepthStencilState(pStencilState.Get(), 1);
	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pStencilView.Get());

	
	DirectX::XMMATRIX viewMatrix, projectionMatrix, worldMatrix;

	pCamera.render();
	viewMatrix = pCamera.getViewMatrix();

	worldMatrix = pworldMatrix;
	projectionMatrix = pprojectionMatrix;

	worldMatrix += DirectX::XMMatrixTranslation(0.f, 0.f, 5.f);
	//Render our loaded Model
	pTextureShader.render(pContext.Get(), worldMatrix, viewMatrix, projectionMatrix);

	pModel.Render();

	worldMatrix += DirectX::XMMatrixTranslation(0.f, 0.f, 15.f);
	//Render our loaded Model
	pTextureShader.render(pContext.Get(), worldMatrix, viewMatrix, projectionMatrix);

	pModel.Render();
	worldMatrix += DirectX::XMMatrixTranslation(0.f, 0.f, 25.f);
	//Render our loaded Model
	pTextureShader.render(pContext.Get(), worldMatrix, viewMatrix, projectionMatrix);

	pModel.Render();
	worldMatrix += DirectX::XMMatrixTranslation(0.f, 0.f, 35.f);
	//Render our loaded Model
	pTextureShader.render(pContext.Get(), worldMatrix, viewMatrix, projectionMatrix);

	pModel.Render();
	worldMatrix += DirectX::XMMatrixTranslation(0.f, 0.f, 45.f);
	//Render our loaded Model
	pTextureShader.render(pContext.Get(), worldMatrix, viewMatrix, projectionMatrix);

	pModel.Render();
	//pModel.Render();


	static std::wstring fpsString = L"FPS: 0";
	static int fpsCounter = 0;
	fpsCounter += 1;
	if (fpsTimer.GetMillisecondsElapsed() > 1000.0f)
	{
		fpsString = L"FPS: " + std::to_wstring(fpsCounter);
		fpsCounter = 0;
		fpsTimer.restart();
	}

	pSpriteBatch->Begin();
	pSpriteFont->DrawString(pSpriteBatch.get(), fpsString.c_str(), DirectX::XMFLOAT2(0.f, 0.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT2(1.0f, 1.f));
	pSpriteBatch->End();



}

void graphics::SetCamera(float x, float y, float z)
{
	pCamera.setPosition(x, y, z);
}

void graphics::AdjustCameraPos(camera::movementType type, float amount)
{
	pCamera.adjustPosition(type, amount);
}

void graphics::SetCameraRotation(float x, float y, float z)
{
	pCamera.setRotation(x, y, z);
}

DirectX::XMMATRIX graphics::getViewMatrix()
{
	return pCamera.getViewMatrix();
}

DirectX::XMMATRIX graphics::getProjectionMatrix()
{
	return pprojectionMatrix;
}

DirectX::XMMATRIX graphics::getWorldMatrix()
{
	return pworldMatrix;
}

RECT graphics::GetRect(HWND hWnd) //get exact dementions of certain window. 
{
	RECT r = { 0 };
	GetWindowRect(hWnd, &r);
	r.right = r.right - r.left;
	r.left = 0;
	r.bottom = r.bottom - r.top;
	r.top = 0;
	return r;
}

void graphics::getWindowDimentions()
{
	RECT r = GetRect(window);
	height = r.bottom;
	width = r.right;
}

bool graphics::create2DTexture()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if(FAILED(pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pStencil)))
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


	pContext->OMSetDepthStencilState(pStencilState.Get(), 1);
		
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(pDevice->CreateDepthStencilView(pStencil.Get(), &depthStencilViewDesc, &pStencilView)))
		return false;


	return true;
}

bool graphics::setRasterState()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
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

	pContext->RSSetState(pRasterState.Get());

	return true;
}

void graphics::setViewport()
{
	D3D11_VIEWPORT viewport;

	// Setup the viewport for rendering.
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	pContext->RSSetViewports(1, &viewport);

}

void graphics::endFrame()
{
	pSwap->Present(1u, 0u);
}
