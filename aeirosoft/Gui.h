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
	input* i;
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
	void SetMainMenu(std::wstring menuName)
	{
		mainMenu = activeMenu = menuName;
	}
	void ActivateMenu(std::wstring menuName)  
	{
		
		activeMenu = menuName;
		vActive.clear();
		for (const auto& m : vMenu)
		{
			if (m->MenuName() == activeMenu)
				vActive.push_back(m);
		}

	}
	bool AddExistingButton(std::wstring MenuName, std::wstring buttonName)
	{
		for (const auto& m : vMenu)
		{
			if (m->type == eButton)
			{
				if (m->objName == buttonName) {
					
					
					vMenu.push_back(new Button(*(Button*)m));
					vMenu[vMenu.size() - 1]->menuName = MenuName;
					return true;
				}
			}
		}
		return false;
	}
	void AddButton( std::function<void(void*)> onClick, void* objptr, std::wstring MenuName, std::wstring fileName, std::wstring highLightFile, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight)
	{
		vMenu.push_back(new Button(g, onClick, objptr, MenuName, fileName, highLightFile, bottomLeft, topRight));

	}
	void AddImage(std::wstring MenuName, std::wstring fileName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight)
	{
		vMenu.push_back(new Image(g, MenuName, fileName, bottomLeft, topRight));
	}

	virtual void Update();
	DirectX::XMFLOAT2 GetCursorToWorldOrtho();
	virtual std::optional<Events*> Queue()
	{
		return this;
	}

	window* w;
private:
	std::wstring mainMenu = L"";
	std::wstring activeMenu = L"";
	HWND m_Window;
	graphics* g;
	enum MenuType { eUndefinedType, eButton, eImage};
	DirectX::XMMATRIX view = g->Get2DViewMatrix();
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

