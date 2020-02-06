#pragma once
#include "framework.h"
#include "Events.h"
#include "graphics.h"
#include <DirectXCollision.h>


class Collidable
{

public:
	Collidable(graphics* g);;
	~Collidable() {};

protected:
	void TransformBounds(DirectX::XMMATRIX m);
	void CreateBoundingBox(std::vector<Vertex> v);
	void CreateBoundingBox(DirectX::XMFLOAT3& size);
	void CreateBoundingBox(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max);
	void AddBoundingBox(DirectX::BoundingBox& bBox);
	void AddBoundingBox(DirectX::XMFLOAT3 size);
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
	void DrawBoundingBox();
	DirectX::BoundingBox GetBounds()
	{
		if (vTransbBox.empty())
			return vOGbBox[0];
		return vTransbBox[0];
	}
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	DirectX::XMMATRIX currentMatrix;
	DirectX::XMVECTOR clickOrigin;
	DirectX::XMVECTOR clickDestination;
	bool CheckRay = false;

	texture redTexture;
	IndexBuffer ib;
	DirectX::BoundingBox MainBox;
	//DirectX::BoundingBox TransbBox;

	std::vector<DirectX::BoundingBox> vOGbBox;
	std::vector<DirectX::BoundingBox> vTransbBox;
	std::vector < DirectX::XMFLOAT3> vMin;
	std::vector < DirectX::XMFLOAT3> vMax;
	graphics* g;
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
			//C->DrawBoundingBox();
			if (C->CheckRay)
			{
				float f = 0.f;
				for (const auto& c : collidable)
				{
					if (c != C)
					{
						DirectX::BoundingBox box = c->GetBounds();
						if (box.Intersects(C->clickOrigin, C->clickDestination, f))
						{
							c->hit = true;
						}
					}
				}
				C->CheckRay = false;
			}
			for (const auto& c : collidable)
			{
				if (c != C)
				{
					if (!c->hit && !c->collision)
					{
						if (C->GetBounds().Contains(c->GetBounds()))
						{
							c->collision = C->collision = true;
						}

					}
				}
			}
		}
		return this;
	}
	void Clear()
	{
		collidable.clear();
	}
	void AddCollidable(Collidable* c)
	{
		collidable.push_back(c);
	}
	std::vector<Collidable*> collidable;
	


};

