#include "gun.h"

gun::gun(graphics* g, ItemStruct iStruct, GunStats gStats, float scale)
	: g(g), Item(g, iStruct, scale), stats(gStats)
{
	Collidable::type = Collidable::Interactable;
	shotSoundEffect = g->CreateSound(gStats._shotSound.c_str());
	shotSound = shotSoundEffect->CreateInstance();
	shotSoundcpy = shotSoundEffect->CreateInstance();
	srand(static_cast <unsigned> (time(0)));
	
}

void gun::interact()
{
	
	return;
}

bool gun::LeftClick()
{

	if (shotTimer.GetSecondsElapsed() == 0 || shotTimer.GetSecondsElapsed() >= stats.fireRate)
	{
		shotTimer.restart();
		if (shotSound->GetState())
		{
			shotSoundcpy->Play();
			shotSound->Stop();

		}
		else if (shotSoundcpy->GetState())
		{
			shotSound->Play();
			shotSoundcpy->Stop();
		}
		else
			shotSound->Play();
		return true;

	}
	return false;

}
