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
	Gui(graphics* g, input* i) :g(g), i(i), view(g->GetViewMatrix())
	{};
	//Gui(graphics* g) :g(g) {};
	~Gui() 
	{
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
	void AddButton( std::function<void(Gui*)> onClick, std::wstring MenuName, std::wstring fileName, std::wstring highLightFile, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight)
	{
		vMenu.push_back(new Button(*g, onClick, MenuName, fileName, highLightFile, bottomLeft, topRight));

	}
	void AddImage(std::wstring MenuName, std::wstring fileName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight)
	{
		vMenu.push_back(new Image(*g, MenuName, fileName, bottomLeft, topRight));
	}

	virtual void Update();
	DirectX::XMFLOAT2 GetCursorToWorldOrtho();
	virtual std::optional<Events*> Queue()
	{
		return this;
	}

private:
	DirectX::XMMATRIX view;
	std::wstring mainMenu = L"";
	std::wstring activeMenu = L"";
	HWND m_Window;
	graphics* g;

	class Menu : protected graphics
	{
	public:
		Menu(const graphics& g, texture t, std::wstring menuName, DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :graphics(g), t(t)
		{
			name = menuName;
			Add(bottomLeft, topRight);
		};
		Menu( const graphics& g, std::wstring filename, std::wstring menuName ,DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :graphics(g), t(pDevice.Get(), filename, aiTextureType::aiTextureType_DIFFUSE)
		{
			name = menuName;
			Add( bottomLeft, topRight);
		};
		virtual ~Menu() {};

		virtual void Collision(DirectX::XMFLOAT2) = 0;
		void Add(DirectX::XMFLOAT2 from, DirectX::XMFLOAT2 to);
		virtual bool OnClick(Gui*, DirectX::XMFLOAT2) = 0;
		virtual void Draw() = 0;
		std::wstring MenuName()
		{
			return name;
		}
	protected:
		std::wstring name;
		std::vector<Vertex> vertices;
		IndexBuffer ib;
		VertexBuffer vb;
		texture t;
		bool collision = false;
		DirectX::XMFLOAT2 from, to;

	};
	class Button : public Menu
	{
	public:
		Button(const graphics& g, std::function<void(Gui*)>onClick, std::wstring menuName, std::wstring filename, std::wstring highLightFile,
			DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :
			Menu(g, filename, menuName, bottomLeft, topRight), highlight(pDevice.Get(), highLightFile, aiTextureType::aiTextureType_DIFFUSE), func(onClick)
		{
			
		};
		~Button() {};
		void Collision(DirectX::XMFLOAT2);
		std::function<void(Gui*)> func;
		virtual bool OnClick(Gui* g, DirectX::XMFLOAT2 mouse)
		{
			if(collision)
				func(g);
			return collision;
		}
		virtual void Draw();
	private:
		texture highlight;
	};
	class Image :public Menu
	{
	public:
		Image(const graphics& g, std::wstring menuName, std::wstring filename,
			DirectX::XMFLOAT2 bottomLeft, DirectX::XMFLOAT2 topRight) :
			Menu(g, filename, menuName, bottomLeft, topRight)
		{};
		virtual void Draw();
		virtual void Collision(DirectX::XMFLOAT2) {};
		virtual bool OnClick(Gui*, DirectX::XMFLOAT2) { return false; };
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

