#pragma once
#include "framework.h"
#include "model.h"
#include "Events.h"
#include "graphics.h"
#include "Timer.h"
struct PR
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
};

struct weaponStats
{
	float recoil = 0.f;
	float spread =0.f;
	float fireRate = 0.1f;
	float recoverRate = 0.f;
	float damage = 0.f;
	float bulletSpeed = 0.f;

	PR ADS;
	PR hip;
	model Bullet;
	model Flash;
};

class weapon : public model
{
public:
	
	weapon(graphics* g, const std::wstring& filename, float scale = 1.f) : g(g)
	{
		init(filename, g, scale);
	}
	~weapon() {};

	void setStats(weaponStats stats)
	{
		this->stats = stats;
	}

	void Render()
	{
		model::Render(this->g->m_TextureShader);
	}
	bool shoot()
	{
		if (!shooting) {
			shooting = true;
			if (shotTimer.GetSecondsElapsed() == 0 || shotTimer.GetSecondsElapsed() >= stats.fireRate)
			{
				shotTimer.restart();
				return true;
				//AddRay();
				//shots += 1;
			}
		}
		shooting = false;
		return false;
			
	}


private:
	graphics* g;
	Timer shotTimer;
	bool shooting = false;
	weaponStats stats;

};

