#pragma once
#include "Events.h"
#include "model.h"
#include "tmpEntity.h"
#include "Timer.h"
class Zombie : public Events, public EntityAi
{
public:
	Zombie(graphics* g) : EntityAi(g, L"Data\\FpsArms\\AnimatedZombie.fbx") ,g(g)
	{
		SetRandomSpawn();
		SetCurrentAnimation("Scene");
	}
	virtual bool Initialize()
	{

		SetRandomSpawn();
		setRotation({ 0,0,0 });
		health = 100;
		attackMode = false;
		return true;
	}
	virtual void Update()
	{
		static float delta = 0;
		delta += deltaTimer.GetMillisecondsElapsed();
		if (delta >= 5.f)
		{
			_Update();
			delta = 0;
		}
		return;
	}
	virtual std::optional<Events*> Queue()
	{
		g->Begin3DScene();

		if (health <= 0) {

			if (!dead)
			{
				SetCurrentAnimation("dead");
				dead = true;
				LookAt(lastLookAt);
			}
			g->TurnOffCulling();
			if (isAnimActive())
				TransformBounds(getWorld());
			Render(g->m_TextureShader);

			g->TurnOnCulling();
			return{};
		}



		SetCurrentAnimation("walking");
		DirectX::XMFLOAT3 mypos = getPosition();
		DirectX::XMFLOAT3 playerPos = g->m_Camera.getPosition();

		DirectX::XMFLOAT3 posDifference = { mypos.x - playerPos.x,0, mypos.z - playerPos.z };

		float delta = deltaTimer.GetMillisecondsElapsed() * .0010f;
		float sight = 600.f;
		if (((posDifference.x <= sight && posDifference.x >= -sight) && posDifference.z <= sight && posDifference.z >= -sight) || attackMode) {
			if (!hit)
				adjustPosition(-posDifference.x * delta, 0, -posDifference.z * delta);
			LookAt(posDifference);
			lastLookAt = posDifference;

		}
		TransformBounds(getWorld());
		if (hit)
		{
			attackMode = true;
			health -= 10;
			hit = false;
		}

		g->TurnOffCulling();
		Render(g->m_TextureShader);
		g->TurnOnCulling();
		deltaTimer.restart();
		return this;
	}
private:
	DirectX::XMFLOAT3 lastLookAt;
	bool attackMode = false;
	bool dead = false;
	Timer deltaTimer;
	graphics* g;
	float health = 100;

	void SetRandomSpawn()
	{
		double delt = deltaTimer.GetMillisecondsElapsed() * (rand() / static_cast <float> (RAND_MAX / 10));
		DirectX::XMFLOAT3 playerpos = g->m_Camera.getPosition();
		float x = (rand() % 1200 + (-800));
		float y = 5;
		float z = (rand() % 1200 + (-800));

		float rando;

		setPosition(x, y, z);


	}
};

