#include "Entity.h"

void Entity::_Update()
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

				c->adjustPosition({ 0, posDifference.y, posDifference.z });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				_Update();
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
				_Update();
				return;
			}
			else
				c->adjustPosition({ 0,0, -posDifference.z });
		}
		if (posDifference.y > 0 || posDifference.y < 0) {
			c->adjustPosition({ 0, -posDifference.y * .4f, 0 });
			TransformBounds(getWorldAtViewMatrix());
			Collidable::Cthis->Check(this);
			if (!collision) {
				falling = false;
				c->adjustPosition({ posDifference.x, 0, posDifference.z });
				TransformBounds(getWorldAtViewMatrix());
				Collidable::Cthis->Check(this);
				_Update();
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

		_Update();

	}
	else {
		if (posDifference.y < 0)
			falling = true;
		loop = 0;

	}

}
