#pragma once
#include "framework.h"
#include "model.h"
#include "Item.h"
#include "Events.h"
#include "graphics.h"
#include "Timer.h"

struct GunStats
{
	bool automatic;
	int vRecoil;
	int hRecoil;
	float spread;
	float fireRate;
	float recoverRate;
	float damage;
	float bulletSpeed;
	float hipPenalty;
	std::wstring _shotSound;
	//model Bullet;
	//model Flash;
};

class gun : public Item
{
public:
	
	gun(graphics* g, ItemStruct iStruct, GunStats gStats, float scale = 1.f);
	~gun()
	{
		shotSoundEffect.release();
		shotSound.release();
	};

	void setStats(GunStats stats)
	{
		this->stats = stats;
	}
	virtual void interact() override;

	virtual bool LeftClick() override;
	DirectX::XMFLOAT2 recoil()
	{
		return {  (float)(rand() % stats.hRecoil + (-stats.hRecoil * .5)),(float)(rand() % stats.vRecoil + 4) };
	}
	GunStats stats;
private:

	graphics* g;

	Timer shotTimer;
	bool shooting = false;

	bool bRecoil = false;
	Timer recoilTimer;

	std::unique_ptr<DirectX::SoundEffect> shotSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> shotSound;
	std::unique_ptr<DirectX::SoundEffectInstance> shotSoundcpy;
	std::vector< std::unique_ptr<DirectX::SoundEffectInstance>> shotSounds;
};

