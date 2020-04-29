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
		TransformBounds(getWorld());
		deadDelta = 0;
		health = 100;
		attackMode = false;
		hit = false;
		dead = false;
		resolve = true;
		return true;
	}
	virtual void Update()
	{

		if (delta >= .25f)
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
				resolve = false;
			}
			g->TurnOffCulling();
			if (isAnimActive())
				TransformBounds(getWorld());
			Render(g->m_TextureShader);

			g->TurnOnCulling();
			deadDelta += deltaTimer.GetSecondsElapsed();
			deltaTimer.restart();
			if (deadDelta >= 10.f)
			{
				Initialize();
				
			}
			else
				return {};
		}



		SetCurrentAnimation("walking");
		DirectX::XMFLOAT3 mypos = getPosition();
		DirectX::XMFLOAT3 playerPos = g->m_Camera.getPosition();

		DirectX::XMFLOAT3 posDifference = { mypos.x - playerPos.x,0, mypos.z - playerPos.z };

		delta = deltaTimer.GetMillisecondsElapsed() * .001f;

		//adjustPosition(0, -30 * delta, 0 );
		float sight = 600.f;

		if (((posDifference.x <= sight && posDifference.x >= -sight) && posDifference.z <= sight && posDifference.z >= -sight) || attackMode) {
			adjustPosition(-posDifference.x * delta, -30 * delta, -posDifference.z * delta);
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
	float deadDelta = 0;
	DirectX::XMFLOAT3 lastLookAt;
	bool attackMode = false;
	bool dead = false;
	Timer deltaTimer;
	graphics* g;
	float health = 100;
	float delta = 0;
	void SetRandomSpawn()
	{
		float x = (rand() % 1200 + (-800));
		float y = 0;
		float z = (rand() % 1200 + (-800));
		setPosition(x, y, z);


	}
};

