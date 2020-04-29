#include "Entity.h"

void Entity::_Update()
{
//	while (!Boundings[0].vAdjustments.empty())
//	{

		for (auto& adjustment : Boundings[0].vAdjustments)
		{
			if (adjustment.y > 0)
				falling = false;
			else if (adjustment.y < 0)
				falling = true;
			adjustPosition(adjustment);
		}
		Boundings[0].vAdjustments.clear();
//		Collidable::Cthis->Check(this);
		TransformBounds(getWorldAtViewMatrix());
//	}
}
