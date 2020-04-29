#pragma once
#include "Collision.h"
#include "input.h"
#include "model.h"
#include <string>


class Entity : public Events, public Collidable, 
	 public model
{
public:

	//virtual bool Initialize() = 0;
	//virtual void Update() = 0;
	//virtual std::optional<Events*> Queue() = 0;


	Entity(graphics* g, input* _i, std::wstring filename, float scale = 1.f) : model(filename, g, scale), _g(g),Collidable(g), c(&g->m_Camera), i(_i)
	{
		Collidable::type = Collidable::Entity;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			Collidable::CreateBoundingOrientedBox(v);
		}
	}

	~Entity() 
	{
		((model*)this)->~model();
	}
	DirectX::XMMATRIX getViewMatrix() { return _g->GetViewMatrix(); }
	DirectX::XMFLOAT3 getPosition()  { return c->getPosition(); }
	DirectX::XMFLOAT3 getRotation()  { return c->getRotation(); }
	void setPosition(DirectX::XMFLOAT3 xmfloat)  { entityHeight = xmfloat.y + 2;  c->setPosition(xmfloat); }
	bool isLeftClick()  { return i->isLeftClick(); };
	bool isRightClick() { return i->isRightClick(); };

	void setPosition(float x, float y, float z)  {
		c->setPosition(x, y, z);
		TransformBounds(getWorldAtViewMatrix());
	}
	virtual bool isKey(UCHAR c)  {
		return i->isKey(c);
	}
	void _Update();
	void adjustPosition(DirectX::XMFLOAT3 pos)
	{
		c->adjustPosition(pos);
	}
	void adjustPosition(camera::movementType type, float velocity)
	{
		if (velocity > 5 && velocity < -5)
			if (velocity > 5)
				velocity = 5;
			else
				velocity = -5;
		prevMove = type;
		recentVelocity = velocity;

		c->adjustPosition(type, velocity);
		TransformBounds(getWorldAtViewMatrix());
		return;
	}

	camera::movementType prevMove;
	float recentVelocity;
	float prevVelocity;
	const std::wstring getName() const { return name; }
	bool falling = false;

protected:
	void CreateBoundingOrientedBox(DirectX::XMFLOAT3 size)
	{
		entityHeight = size.y;
		Collidable::CreateBoundingOrientedBox(size);
	}
	DirectX::XMMATRIX getWorldAtViewMatrix()
	{
		DirectX::XMFLOAT3 rot = c->getRotation();
		DirectX::XMFLOAT3 pos = c->getPosition();
		return  DirectX::XMMatrixRotationRollPitchYaw(0, rot.y, 0) * DirectX::XMMatrixTranslation(pos.x, pos.y - entityHeight, pos.z);
	}

private:

	float entityHeight;

	input* i;
	camera* c;
	std::wstring name;
	graphics* _g;


};
