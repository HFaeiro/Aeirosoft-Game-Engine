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
	int vRecoil = 12.f;
	int hRecoil = 20.f;
	float spread =0.f;
	float fireRate = 0.125f;
	float recoverRate = .0925f;
	float damage = 0.f;
	float bulletSpeed = 0.f;
	float hipPenalty = 1.2f;
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
		srand(static_cast <unsigned> (time(0)));
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
		if (shotTimer.GetSecondsElapsed() == 0 || shotTimer.GetSecondsElapsed() >= stats.fireRate)
			{
				shotTimer.restart();
				
				return true;

			}
		return false;
			
	}
	DirectX::XMFLOAT2 recoil()
	{
		return { (float)(rand() % stats.vRecoil + 4 ), (float)(rand() % stats.hRecoil + (-stats.hRecoil * .5)) };
	}
	weaponStats stats;
private:
	graphics* g;
	Timer shotTimer;
	bool shooting = false;

	bool bRecoil = false;
	Timer recoilTimer;

};

