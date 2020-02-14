#pragma once
#include "Collision.h"
#include "input.h"
#include "model.h"
#include <string>

class Entity : public Collidable, 
	 public model
{
public:
	Entity(graphics* g, input* _i, std::wstring filename, float scale = 1.f) : model(filename, g, scale), _g(g),Collidable(g), c(&g->m_Camera), i(_i)
	{
		Collidable::type = Collidable::Entity;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}

	}
	DirectX::XMMATRIX getViewMatrix() { return _g->GetViewMatrix(); }
	DirectX::XMFLOAT3 getPosition()  { return c->getPosition(); }
	DirectX::XMFLOAT3 getRotation()  { return c->getRotation(); }
	void setPosition(DirectX::XMFLOAT3 xmfloat)  { playerHeight = xmfloat.y + 2;  c->setPosition(xmfloat); }
	virtual bool isLeftClick()  { return i->isLeftClick(); };
	virtual bool isRightClick() { return i->isRightClick(); };
	void setPosition(float x, float y, float z)  { playerHeight = y + 2; c->setPosition(x, y, z);}
	virtual bool isKey(UCHAR c)  {
		return i->isKey(c);
	}

	void Update()
	{
		static int loop = 0;
		if (loop == 0)
			prevVelocity = recentVelocity;
		if (collision) {
			/*for (const auto& cWith : collidedWith)
			{
				DirectX::XMFLOAT3 corners[8];
				if (cWith.type == Collidable::Object)
				{
					cWith.GetBounds().GetCorners(corners);
				}*/

			
			DirectX::XMFLOAT3 movedPos = c->getPosition();
			//c->adjustPosition(prevMove, -recentVelocity);
			c->revertView();
			DirectX::XMFLOAT3 prevPos = c->getPosition();
			c->setPosition(prevPos);
			//DirectX::XMFLOAT3 posDifference = { movedPos.x - prevPos.x, movedPos.y - prevPos.y, movedPos.z - prevPos.z };
			DirectX::XMFLOAT3 posDifference = {prevPos.x - movedPos.x,prevPos.y - movedPos.y,prevPos.z - movedPos.z };
			c->adjustPosition({posDifference.x * .005f, posDifference.y * .005f, posDifference.z * .005f});
			
			if (loop > 50)
			{
				loop = 0;




				return;

			}
			/*if (loop < 5) {
				prevVelocity *= .5;
				c->adjustPosition(prevMove, prevVelocity);
			}
			else
			{
				if (loop == 5)
					prevVelocity = recentVelocity;
				else
					prevVelocity *= 1.5;
				c->adjustPosition(prevMove, -prevVelocity);
			}*/
			collision = false;
			TransformBounds(getWorldAtViewMatrix());
			Collidable::Cthis->Check(this);
			++loop;
			
			//}
			Update();
			
		}
		else
			loop = 0;
		
	}

	void adjustPosition(camera::movementType type, float velocity) 
	{
		if (!collision) {
			prevMove = type;
			recentVelocity = velocity;
		
			c->adjustPosition(type, velocity);
			TransformBounds(getWorldAtViewMatrix());
		}
		return;
	}

	~Entity() {}
	camera::movementType prevMove;
	float recentVelocity;
	float prevVelocity;
	const std::wstring getName() const { return name; }

private:
	DirectX::XMMATRIX getWorldAtViewMatrix()
	{
		DirectX::XMFLOAT3 rot = c->getRotation();
		DirectX::XMFLOAT3 pos = c->getPosition();
		return  DirectX::XMMatrixRotationRollPitchYaw(0, rot.y, 0) * DirectX::XMMatrixTranslation(pos.x, pos.y - playerHeight + 4, pos.z);
	}
	float playerHeight;

	input* i;
	camera* c;
	std::wstring name;
	graphics* _g;


};
class EntityAi : public Collidable, public model
{
public:
	EntityAi(graphics* g, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g)
	{
		Collidable::type = Collidable::EntityAi;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}
	}
	~EntityAi() {}

	const std::wstring getName() const { return name; }

private:
	std::wstring name;

};
class EntityObject : public Collidable, public model
{
public:
	EntityObject(graphics* g, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g)
	{
		Collidable::type = Collidable::Object;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}
	}
	~EntityObject() {}

	const std::wstring getName() const { return name; }

private:
	std::wstring name;

};