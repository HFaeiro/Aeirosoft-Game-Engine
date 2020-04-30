#include "tmpEntity.h"

void EntityAi::_Update()
{
	
	for (auto& adjustment : Boundings[0].vAdjustments)
	{
		adjustPosition(adjustment);
	}
		
	TransformBounds(getWorld());
	Boundings[0].vAdjustments.clear();

}
