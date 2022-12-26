#pragma once
#include "Events.h"
#include "Ai.h"
#include <time.h>
#include "timer.h"
#include <sstream>
class MovingAimBox : public Events , public Ai

{
public:
	MovingAimBox(const MovingAimBox& m) : Ai(m)
	{
		this->g = m.g;
	}
	MovingAimBox(graphics* g);
	~MovingAimBox() {};

	virtual bool Initialize()
	{
		SetRandomSpawn();
		//UpdateScale({ 20, 20, 20});
		health = 100;
		hit = false;
		delta.restart();
		aliveTime.restart();
		deadDelta = 0;
		return true;
	}
	virtual void Update()
	{
		Ai::Update();
	//	g->TurnOffCulling();
		Render(g->m_TextureShader);
	//	g->TurnOnCulling();
		prevDirection = direction;
		

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
	int health = 100;
	bool dead = false;
	DirectX::XMFLOAT3 lastLookAt;
	float deadDelta = 0;
};

