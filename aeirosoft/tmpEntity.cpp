#include "tmpEntity.h"

void EntityAi::_Update()
{


	static int loop = 0;
	static DirectX::XMFLOAT3 lastResort;
	DirectX::XMFLOAT3 movedPos = getPosition();

	revertWorld();
	DirectX::XMFLOAT3 prevPos = getPosition();
	if (loop == 0)
		lastResort = prevPos;
	setPosition(movedPos);
	DirectX::XMFLOAT3 posDifference = { movedPos.x - prevPos.x, movedPos.y - prevPos.y, movedPos.z - prevPos.z };
	//DirectX::XMFLOAT3 posDifference = {prevPos.x - movedPos.x,prevPos.y - movedPos.y,prevPos.z - movedPos.z };
	if (collision) {
	//	collision = false;
	//	setPosition(prevPos);
	//	
		float newPosMult = .9f * (loop == 0 ? 1 : loop * .0099f);

		if (posDifference.x) {
			adjustPosition({ posDifference.x, 0, 0 });
			TransformBounds(getWorld());
			Collidable::Cthis->Check(this);
			if (!collision) {

				adjustPosition({ 0, posDifference.y, posDifference.z });
				TransformBounds(getWorld());
				Collidable::Cthis->Check(this);
				_Update();
				return;
			}
			else
				adjustPosition({ -posDifference.x, 0, 0 });
		}
		if (posDifference.z) {
			adjustPosition({ 0,0, posDifference.z });
			TransformBounds(getWorld());
			Collidable::Cthis->Check(this);
			if (!collision) {
				adjustPosition({ posDifference.x , posDifference.y, 0 });
				TransformBounds(getWorld());
				Collidable::Cthis->Check(this);
				_Update();
				return;
			}
			else
				adjustPosition({ 0,0, -posDifference.z });
		}
		if (posDifference.y > 0 || posDifference.y < 0) {
			adjustPosition({ 0, -posDifference.y * .4f, 0 });
			TransformBounds(getWorld());
			Collidable::Cthis->Check(this);
			if (!collision) {
				//falling = false;
				adjustPosition({ posDifference.x, 0, posDifference.z });
				TransformBounds(getWorld());
				Collidable::Cthis->Check(this);
				_Update();
				return;
			}
			else
				adjustPosition({ 0, posDifference.y * .4f, 0 });
		}


		if (posDifference.x == 0 && posDifference.y == 0 && posDifference.z == 0)
		{
			loop = 0;
			setPosition(lastResort);



			//adjustPosition({ 0,.001f,0 });
			collision = false;
			return;
		}
		if (loop > 10)
		{
			loop = 0;


			setPosition(lastResort);
			collision = false;
			return;

		}



		DirectX::XMFLOAT3 newPos = { posDifference.x * newPosMult,  posDifference.y * newPosMult ,  posDifference.z * newPosMult };
		adjustPosition(newPos);
		collision = false;
		TransformBounds(getWorld());
		Collidable::Cthis->Check(this);
		++loop;

		_Update();

	}
	else {
		//if (posDifference.y < 0)
		//	falling = true;
		loop = 0;

	}

}
