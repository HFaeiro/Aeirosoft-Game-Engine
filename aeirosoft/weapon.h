#pragma once
#include "framework.h"
#include "model.h"
#include "Events.h"
#include "graphics.h"
struct weaponStats
{
	float recoil;
	float spread;
	float fireRate;
	float recoverRate;
	float damage;
	float bulletSpeed;
	model Bullet;
	model Flash;
};

class weapon : public model
{
public:
	
	weapon(graphics* g , const std::wstring& filename) : g(g)
	{
		init(filename, g);
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

