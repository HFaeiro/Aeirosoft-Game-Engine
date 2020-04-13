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

	void _Update()
	{
		

		static int loop = 0;
		static DirectX::XMFLOAT3 lastResort;
		DirectX::XMFLOAT3 movedPos = c->getPosition();

		c->revertView();
		DirectX::XMFLOAT3 prevPos = c->getPosition();
		if (loop == 0)
			lastResort = prevPos;
		c->setPosition(movedPos);
		DirectX::XMFLOAT3 posDifference = { movedPos.x - prevPos.x, movedPos.y - prevPos.y, movedPos.z - prevPos.z };

		


		if (collision) {
			collision = false;
			c->setPosition(prevPos);
			//DirectX::XMFLOAT3 posDifference = {prevPos.x - movedPos.x,prevPos.y - movedPos.y,prevPos.z - movedPos.z };
			float newPosMult = .1999f * (loop == 0 ? 1 : loop * .0099f);
			
			if (posDifference.x) {
				c->adjustPosition({ posDifference.x, 0, 0 });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				if (!collision) {

					c->adjustPosition({ 0, posDifference.y, posDifference.z});
					TransformBounds(getWorldAtViewMatrix());
					Collidable::Cthis->Check(this);
					Update();
					return;
				}
				else
					c->adjustPosition({ -posDifference.x, 0, 0 });
			}
			if (posDifference.z) {
				c->adjustPosition({ 0,0, posDifference.z });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				if (!collision) {
					c->adjustPosition({ posDifference.x , posDifference.y, 0 });
					TransformBounds(getWorldAtViewMatrix());
					Collidable::Cthis->Check(this);
					Update();
					return;
				}
				else
					c->adjustPosition({ 0,0, -posDifference.z });
			}
			if (posDifference.y > 0 || posDifference.y < 0) {
				/*c->adjustPosition({ 0, posDifference.y, 0 });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				if (!collision) {
					falling = true;
					c->adjustPosition({ posDifference.x, 0, posDifference.z });
					TransformBounds(getWorldAtViewMatrix());
					Collidable::Cthis->Check(this);
					Update();
					return;
				}
				c->adjustPosition({ 0, -posDifference.y, 0 });	
				collision = false;*/
				c->adjustPosition({ 0, -posDifference.y * .4f, 0 });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				if (!collision) {
					falling = false;
					c->adjustPosition({posDifference.x, 0, posDifference.z});
					TransformBounds(getWorldAtViewMatrix());
					Collidable::Cthis->Check(this);
					Update();
					return;
				}
				else
					c->adjustPosition({ 0, posDifference.y * .4f, 0 });
			}


			if (posDifference.x == 0 && posDifference.y == 0 && posDifference.z == 0)
			{
				loop = 0;
				c->setPosition(movedPos);



				c->adjustPosition({ 0,.001f,0 });
				collision = false;
				return;
			}
			if (loop > 7)
			{
				loop = 0;


				c->setPosition(lastResort);
				collision = false;
				return;

			}



			DirectX::XMFLOAT3 newPos = { posDifference.x * newPosMult,  posDifference.y * newPosMult ,  posDifference.z * newPosMult };
			c->adjustPosition(newPos);
			collision = false;
			TransformBounds(getWorldAtViewMatrix());
			Collidable::Cthis->Check(this);
			++loop;

			Update();

		}
		else {
		if (posDifference.y < 0)
			falling = true;
			loop = 0;
			
		}

	}

	void adjustPosition(camera::movementType type, float velocity) 
	{
		if (!collision && (velocity < 5 && velocity > -5) ) {
			prevMove = type;
			recentVelocity = velocity;
		
			c->adjustPosition(type, velocity);
			TransformBounds(getWorldAtViewMatrix());
		}
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
