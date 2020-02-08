#pragma once
#include "framework.h"
#include "model.h"
#include "Events.h"
#include "graphics.h"

struct PR
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
};

struct weaponStats
{
	float recoil;
	float spread;
	float fireRate;
	float recoverRate;
	float damage;
	float bulletSpeed;

	PR ADS;
	PR hip;
	model Bullet;
	model Flash;
};

class weapon : public model
{
public:
	
	weapon(graphics* g , const std::wstring& filename, float scale = 1.f) : g(g)
	{
		init(filename, g, scale);
	}
	~weapon() {};

	void setStats(weaponStats stats)
	{
		this->stats = stats;
	}
	void setBarrelTip(DirectX::XMFLOAT3);

	void Render()
	{
		((model*)this)->Render(this->g->m_TextureShader);
	}
	void shoot()
	{

	}


private:
	graphics* g;
	//TextureShader tShader;
	weaponStats stats;

};

