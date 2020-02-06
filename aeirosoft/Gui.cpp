#include "Gui.h"
#include <sstream>
void Gui::Button::Collision(DirectX::XMFLOAT2 p)
{

	//need to fix from and to points on window resize.
	//thinks they're always in the same place. 
	//multiply points by difference in old window size to new?
	//do i have to do this on 3d as well?

	if (p.x > from.x && p.y > from.y) {
		if (p.x < to.x && p.y < to.y)
		{
			collision = true;
			return;
		}
	}
	
		collision = false;



}

void Gui::Menu::Add( DirectX::XMFLOAT2 from, DirectX::XMFLOAT2 to)
{

	this->from = from;
	this->to = to;
	vertices.emplace_back(from.x, from.y, 0.f, 0.f, 1.f);// 0  bottom left
	vertices.emplace_back(to.x, to.y, 0.f, 1.f, 0.f); //1 top right
	vertices.emplace_back(from.x, to.y, 0.f, 0, 0);			//2 top left
	
	
	
	vertices.emplace_back(from.x, from.y, 0.f, 0.f, 1.f);// 0  bottom left
	vertices.emplace_back(to.x, to.y, 0.f, 1.f, 0.f); //1 top right
	vertices.emplace_back(to.x, from.y, 0.f, 1.f, 1.f); //3 bottom right
//	vertices.emplace_back(to.x, from.y, 0.f, 1.f, 1.f); //3 bottom right

	DWORD indecies[] =
	{
	
		0,2,1,
			3,4,5,
	};



	CreateIndexAndVectorBuffers(vertices, indecies, vb, ib);

}

void Gui::Button::Draw()
{
	Begin2DScene();
	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if(!collision)
		pContext->PSSetShaderResources(0, 1, t.GetTextureResourceViewAddr());
	else
		pContext->PSSetShaderResources(0, 1, highlight.GetTextureResourceViewAddr());
	pContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.GetStridePtr(), &offset);
	pContext->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->DrawIndexed(6, 0, 0);



}

void Gui::Image::Draw()
{
	
	Begin2DScene();
	UINT offset = 0;
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->PSSetShaderResources(0, 1, t.GetTextureResourceViewAddr());
	pContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.GetStridePtr(), &offset);
	pContext->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->DrawIndexed(6, 0, 0);
}

void Gui::Update()
{
	DirectX::XMFLOAT2 mouse = GetCursorToWorldOrtho();
	if (i->isKey(DIK_ESCAPE))
		if (activeMenu != mainMenu)
			ActivateMenu(mainMenu);

	for (const auto& m : vActive)
	{
		m->Draw();



		m->Collision({ mouse.x, mouse.y });
		if (i->isLeftClick())
		{
			if (m->OnClick(this, { mouse.x, mouse.y }))
				break;
		}

	}

	std::wstringstream wss;
	wss << mouse.x << " " << mouse.y;


	g->pSpriteBatch->Begin();
	g->pSpriteFont->DrawString(g->pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 20));
	g->pSpriteFont->DrawString(g->pSpriteBatch.get(), i->fpsString.c_str(), DirectX::XMFLOAT2(0, 0));
	g->pSpriteBatch->End();

}

DirectX::XMFLOAT2 Gui::GetCursorToWorldOrtho()
{
	POINT p;
	//GetCursorPos(&p);
	i->GetMouse(p);
	
		RECT r = helper::window::GetRect(m_Window);
		//ScreenToClient(m_Window, &p);
		
		DirectX::XMMATRIX world = g->GetWorldMatrix();
		DirectX::XMVECTOR clickOrigin = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 0.f, 0.f),
			0, 0,
			r.right, r.bottom,
			0, 1.f,
			g->GetOrthoMatrix(),
			view,
			world);

		DirectX::XMFLOAT3 origin;
		DirectX::XMStoreFloat3(&origin, clickOrigin);

		return { origin.x, origin.y };

}