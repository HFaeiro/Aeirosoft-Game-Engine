#pragma once
#include "framework.h"
#include "Events.h"
#include "graphics.h"
#include "aeBounding.h"
#include <variant>


class Collidable
{
public:
	Collidable(graphics* g);
	~Collidable() {};
	void TransformBounds(DirectX::XMMATRIX m);
	enum Type { Entity, EntityAi, Object };
	Type type;
	std::vector<aeBounding> GetBounds()const
	{
		//if (vTransbBox.empty())
		//	return vOGbBox;
		if(Boundings.size())
			return Boundings;
	}

	DirectX::BoundingSphere GetBoundSphere()const
	{
		if (Boundings.size())
			return Boundings[0].getSphere();
	}
	bool resolve = true;
protected:
	void CreateBoundingOrientedBox(std::vector<std::vector<Vertex>> v, std::vector<DirectX::XMMATRIX*> transforms);
	void CreateBoundingOrientedBox(std::vector < std::vector<Vertex>> v);
	void CreateBoundingOrientedBox(std::vector<Vertex> v);
	void CreateBoundingOrientedBox(DirectX::XMFLOAT3& size);
	/*void AddBoundingOrientedBox(DirectX::BoundingOrientedBox& bBox);
	void AddBoundingOrientedBox(DirectX::XMFLOAT3 size);*/
	void AddRay()
	{
		RECT r = helper::window::GetRect(g->GetWindow());

		POINT p;
		p.x = r.right * .5;
		p.y = r.bottom * .5;



		DirectX::XMMATRIX projection = g->GetProjectionMatrix();
		DirectX::XMMATRIX view = g->GetViewMatrix();
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

		clickOrigin = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 0.f, 0.f),
			0, 0,
			r.right, r.bottom,
			0, 1.f,
			projection,
			view,
			world);

		DirectX::XMVECTOR dest = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 1.f, 0.f),
			0, 0,
			r.right, r.bottom,
			0, 1.f,
			projection,
			view,
			world);

		clickDestination = DirectX::XMVectorSubtract(dest, clickOrigin);
		clickDestination = DirectX::XMVector3Normalize(clickDestination);
		CheckRay = true;

	}
	bool hit = false;
	bool collision = false;
private:

	friend class Collision;
#ifdef _DEBUG
	void CreateTexture();
	void DrawBoundingOrientedBox();
#endif // _DEBUG






	std::vector<DirectX::XMFLOAT3> g_corners;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	DirectX::XMMATRIX currentMatrix = {};
	DirectX::XMVECTOR clickOrigin = {};
	DirectX::XMVECTOR clickDestination = {};
	bool CheckRay = false;

	texture redTexture;
	texture blueTexture;
	IndexBuffer ib;
	bool hasMainBox = false;
	std::vector <DWORD> Indecies;
	graphics* g;
protected:
	
	std::vector<aeBounding> Boundings;
	Collision* Cthis;
	std::vector<aeBounding> collidedWith;
	std::vector<Vertex> vertices;
};

class Collision : public Events
{
public:

	virtual bool Initialize()
	{
		return true;
	}
	virtual void Update()
	{

		return;
	}
	virtual std::optional<Events*> Queue();
	void Clear()
	{
		collidable.clear();
	}
	void AddCollidable(Collidable* c)
	{
		if (c->Cthis != this)
			c->Cthis = this;
		collidable.push_back(c);
	}
	void Check(Collidable* C);


	std::vector<Collidable*> collidable;
	
private:


};

