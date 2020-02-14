#pragma once
#include "framework.h"
#include "Events.h"
#include "graphics.h"
#include <DirectXCollision.h>
#include <variant>
class Collidable
{
	
public:
	Collidable(graphics* g);;
	~Collidable() {};
	void TransformBounds(DirectX::XMMATRIX m);
	enum Type { Entity, EntityAi, Object };
	Type type;
	DirectX::BoundingOrientedBox GetBounds()const
	{
		if (vTransbBox.empty())
			return vOGbBox[0];
		return vTransbBox[0];
	}

	DirectX::BoundingSphere GetBoundSphere()const
	{
		return bSphere;
	}
protected:

	void CreateBoundingOrientedBox(std::vector<Vertex> v);
	void CreateBoundingOrientedBox(DirectX::XMFLOAT3& size);
	void CreateBoundingOrientedBox(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max);
	void AddBoundingOrientedBox(DirectX::BoundingOrientedBox& bBox);
	void AddBoundingOrientedBox(DirectX::XMFLOAT3 size);
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
	void CreateTexture();
	void DrawBoundingOrientedBox();


	
	

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	DirectX::XMMATRIX currentMatrix = {};
	DirectX::XMVECTOR clickOrigin = {};
	DirectX::XMVECTOR clickDestination = {};
	bool CheckRay = false;

	texture redTexture;
	IndexBuffer ib;
	DirectX::BoundingOrientedBox MainBox;
	//DirectX::BoundingOrientedBox TransbBox;
	std::vector < DirectX::XMFLOAT3> vMin;
	std::vector < DirectX::XMFLOAT3> vMax;
	graphics* g;
protected:

	std::vector<DirectX::BoundingOrientedBox> vOGbBox;
	std::vector<DirectX::BoundingOrientedBox> vTransbBox;
	DirectX::BoundingSphere bSphere;


	Collision* Cthis;
	std::vector<Collidable> collidedWith;
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
	virtual std::optional<Events*> Queue()
	{
		for (const auto& C : collidable)
		{
#ifdef _DEBUG
			C->DrawBoundingOrientedBox();
#endif
			if (C->type != Collidable::Type::Object)
			{
				if (C->CheckRay)
				{
					float f = 0.f;
					for (const auto& c : collidable)
					{
						if (c != C)
						{
							
							DirectX::BoundingOrientedBox box = c->GetBounds();
							if (box.Intersects(C->clickOrigin, C->clickDestination, f))
							{
								c->hit = true;
							}
						}
					}
					C->CheckRay = false;
				}
				Check(C);
			}
		}
		return {};
	}
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
	void Check(Collidable* C)
	{
		for (const auto& c : collidable)
		{
			if (c != C)
			{
				if (C->type == Collidable::Type::Entity) {
					auto box = C->GetBoundSphere();
					if (box.Contains(c->GetBounds()))
					{
						C->collision = true;
					}
				}
				else {
					auto box = C->GetBounds();

					if (box.Contains(c->GetBounds()))
					{
						C->collision = true;
					}
				}
			}
		}
	}


	std::vector<Collidable*> collidable;
	
private:


};

