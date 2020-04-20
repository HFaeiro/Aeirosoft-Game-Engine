#pragma once
#include "Events.h"
#include "tmpEntity.h"
#include <time.h>
#include "timer.h"
#include <sstream>
class MovingAimBox : public Events , public EntityAi

{
public:
	MovingAimBox(const MovingAimBox& m) : EntityAi(m)
	{
		this->g = m.g;
	}
	MovingAimBox(graphics* g);
	~MovingAimBox() {};

	virtual bool Initialize()
	{
		SetRandomSpawn();
		//UpdateScale({ 20, 20, 20});
		delta.restart();
		aliveTime.restart();
		return true;
	}
	virtual void Update()
	{

		g->TurnOffCulling();
		Render(g->m_TextureShader);
		g->TurnOnCulling();
		prevDirection = direction;
		model::SetCurrentAnimation("wiggle");
		return;
	}
	virtual std::optional<Events*> Queue();
	int GetHits()
	{
		return hits;
	}
private:
	void UpdateMove();
	void SetRandomMove();
	void SetRandomSpawn();
	double moveTime = 0.f;
	double timeMoving = 0.f;
	Timer aliveTime;
	int hits = 0;
	DWORD prevDirection = -1;
	double m_moveSpeed;
	DWORD direction;
	graphics* g;
	Timer delta;
};

