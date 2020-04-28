#include "tmpEntity.h"

void EntityAi::_Update()
{
	std::vector<DirectX::XMFLOAT3> adjustments = Boundings[0].vAdjustments;
	for (auto& adjustment : Boundings[0].vAdjustments)
	{
		adjustPosition(adjustment);
		//adjustment = { 0,0,0 };
		//Boundings[0].vAdjustments.erase(Boundings[0].vAdjustments.begin());
	}
		
	TransformBounds(getWorld());
	Boundings[0].vAdjustments.clear();

}
