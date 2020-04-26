#include "Entity.h"

void Entity::_Update()
{
	for (auto adjustment : Boundings[0].vAdjustments)
	{
		if (adjustment.y > 0)
			falling = false;
		else if( adjustment.y < 0)
			falling = true;
		adjustPosition(adjustment);
	}
	Boundings[0].vAdjustments.clear();
	TransformBounds(getWorldAtViewMatrix());

}
