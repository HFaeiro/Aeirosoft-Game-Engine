#include "weapon.h"

weapon::weapon(graphics* g, const std::wstring& filename, const std::wstring& _shotSound, float scale) : g(g)
{
	init(filename, g, scale);
	shotSoundEffect = g->CreateSound(_shotSound.c_str());
	shotSound = shotSoundEffect->CreateInstance();
	shotSoundcpy = shotSoundEffect->CreateInstance();
	srand(static_cast <unsigned> (time(0)));
}

bool weapon::shoot()
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
		//if (shotSounds.size())
		//{
		//	for(const auto& sound : shotSounds)
		//		if(!sound->GetState())

		//			
		//}
		//else
		//{
		//	shotSounds.emplace_back(shotSoundEffect->CreateInstance());
		//}
		return true;

	}
	return false;

}
