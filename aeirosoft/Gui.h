#pragma once
#include "framework.h"
#include "graphics.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "texture.h"
#include <functional>
#include "Events.h"
#include "input.h"

class Gui : public Events
{
public:
	Gui(graphics* g, input* i, window* w) :g(g), i(i), w(w)
	{};
	//Gui(graphics* g) :g(g) {};
	~Gui() 
	{
		for (auto m : vMenu)
		{
			m->~Menu();
		}
	}
	virtual bool Initialize()
	{
		m_Window = g->GetWindow();

		vActive.clear();
		if (!activeMenu.length())
			return false;
		for (const auto& m : vMenu)
		{
			if (m->MenuName() == activeMenu)
				vActive.push_back(m);
		}

		return true;
	}

	virtual void Update();
	virtual std::optional<Events*> Queue()
	{
		return this;
	}

	void SetMainMenu(std::wstring menuName);
	void ActivateMenu(std::wstring menuName);
	bool AddExistingButton(std::wstring MenuName, std::wstring buttonName);
	void AddButton(std::function<void(void*)> onClick, void* objptr, std::wstring MenuName, std::wstring fileName, std::wstring highLightFile, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight);
	void AddImage(std::wstring MenuName, std::wstring fileName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight);


	DirectX::XMFLOAT2 GetCursorToWorldOrtho();

	std::wstring GetActiveMenu()
	{
		return activeMenu;
	}
	window* w;
	input* i;
private:
	std::wstring mainMenu = L"";
	std::wstring activeMenu = L"";
	HWND m_Window;
	graphics* g;
	enum MenuType { eUndefinedType, eButton, eImage};
	DirectX::XMMATRIX view = g->Get2DViewMatrix();
	class Font
	{
	public:
		Font(graphics* g, std::wstring font) : fontName(font), g(g)
		{
			pSpriteFont = std::make_unique<DirectX::SpriteFont>(g->GetDevice().Get(), font.c_str());
			fontName = helper::strings::GetDirectoryFromPath(font);
		}

		~Font()
		{
			pSpriteFont.release();
		}
		void Render(wchar_t* text, DirectX::XMFLOAT2 location)
		{
			pSpriteFont->DrawString(g->pSpriteBatch.get(), text, DirectX::XMFLOAT2(0, 40));
		}
		std::wstring fontName;
	private:
		graphics* g;
		std::unique_ptr<DirectX::SpriteFont> pSpriteFont;

	};
	class Text
	{
	public:
		Text(graphics* g, std::wstring name, std::wstring _font, DirectX::XMFLOAT2 location) : name(name), location(location), g(g)
		{
			pFont = new Font(g, _font);
		}
		Text(graphics* g, std::wstring name, Font* font, DirectX::XMFLOAT2 location)
		{
			pFont = font;
		}
		~Text() {};

		void Render()
		{
			pFont->Render(textToRender, location);
		}
		Font* pFont;
	private:
		wchar_t* textToRender;
		DirectX::XMFLOAT2 location;
		std::wstring name;
		graphics* g;
		//	pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice.Get(), L"Data\\Fonts\\hFontSm.spritefont");
		//		m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), wssAcc.str().c_str(), DirectX::XMFLOAT2(0, 40));
	};

	class Menu
	{
	public:
		Menu(graphics* g, texture t, std::wstring _menuName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :g(g), t(t)
		{
			menuName = _menuName;
			Add(bottomLeft, topRight);
		};
		Menu(graphics* g, std::wstring filePath, std::wstring _menuName ,DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :g(g), t(g->GetDevice().Get(), filePath, aiTextureType::aiTextureType_DIFFUSE)
		{
			menuName = _menuName;
			objName = helper::strings::GetNameFromPath(filePath);
			Add( bottomLeft, topRight);
		};
		virtual ~Menu() 
		{
			this->t.~texture();
		};

		virtual void Collision(DirectX::XMFLOAT2) = 0;
		void Add(DirectX::XMFLOAT2 from, DirectX::XMFLOAT2 to);
		virtual bool OnClick(DirectX::XMFLOAT2) = 0;
		virtual void Draw() = 0;
		std::wstring MenuName()
		{
			return menuName;
		}
		friend Gui;
	protected:
		std::wstring menuName;
		std::wstring objName;
		std::vector<Vertex> vertices;
		IndexBuffer ib;
		VertexBuffer vb;
		texture t;
		bool collision = false;
		DirectX::XMFLOAT2 from, to;
		graphics* g;
		Microsoft::WRL::ComPtr < ID3D11DeviceContext	> pContext = g->GetDeviceContext();
		MenuType type = eUndefinedType;
		
	};
	class Button : public Menu
	{
	public:
		Button(graphics* g, std::function<void(void*)>onClick, void* _objptr, std::wstring menuName, std::wstring filename, std::wstring highLightFile,
			DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :
			Menu(g, filename, menuName, bottomLeft, topRight), highlight(g->GetDevice().Get(), highLightFile, aiTextureType::aiTextureType_DIFFUSE), func(onClick), objptr(_objptr)
		{
			type = eButton;
		};
		~Button() {};
		void Collision(DirectX::XMFLOAT2);
		std::function<void(void*)> func;
		virtual bool OnClick(DirectX::XMFLOAT2 mouse)
		{
			if(collision)
				func(objptr);
			return collision;
		}
		virtual void Draw();
	private:
		void* objptr = nullptr;
		texture highlight;
		
	};
	class Image :public Menu
	{
	public:
		Image(graphics* g, std::wstring menuName, std::wstring filename,
			DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :
			Menu(g, filename, menuName, bottomLeft, topRight)
		{
			type = eImage;
		};
		virtual void Draw();
		virtual void Collision(DirectX::XMFLOAT2) {};
		virtual bool OnClick(DirectX::XMFLOAT2) { return false; };
	private:
		
	};
	//class Edit :public Menu
	//{
	//public:
	//	Edit(const graphics& g, std::wstring filename);
	//private:
	//	bool Collision(DirectX::XMFLOAT2);
	//	virtual void Add(std::wstring fileName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight);
	//	virtual void OnClick();
	//	virtual void Draw();
	//};

	std::vector<Menu*> vMenu;
	std::vector<Menu*> vActive;


};

