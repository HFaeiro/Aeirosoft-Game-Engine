#pragma once
#include "Events.h"
#include "Entity.h"
#include <time.h>
#include "timer.h"
#include <sstream>
class MovingAimBox : public Events , public Entity

{
public:
	MovingAimBox(graphics* g): g(g) ,Entity(g, L"Data\\Objects\\Target\\target.obj", 5) { srand(static_cast <unsigned> (time(0))); };
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
		return;
	}
	virtual std::optional<Events*> Queue()
	{

		if (hit) {
			Initialize();
			hits += 1;
		}
		if (aliveTime.GetSecondsElapsed() >= 25) {
			Initialize();
			
		}
		if (collision) {
			revertWorld();
			m_moveSpeed = -m_moveSpeed;
			collision = false;
			
		}
		hit = false;
		if (moveTime <= timeMoving) {
			SetRandomMove();
			timeMoving = 0.f;
		}
		UpdateMove();
		TransformBounds(getWorld());
		return this;
	}
	int GetHits()
	{
		return hits;
	}
private:
	void UpdateMove()
	{
		DirectX::XMFLOAT3 pos = getPosition();
		double elap = delta.GetSecondsElapsed();
		double moveSpeed = m_moveSpeed * elap;

		switch (direction)
		{
		case 0:
			adjustPosition(moveSpeed, 0, 0);
			break;
		case 1:
			adjustPosition(-moveSpeed, 0, 0);
			break;
		case 2:
			adjustPosition(0, moveSpeed, 0);
			break;
		case 3:
			adjustPosition(0, -moveSpeed, 0);
			break;
		case 4:
			adjustPosition(0, 0, moveSpeed);
			break;
		case 5:
			adjustPosition(0, 0, -moveSpeed);
			break;
		case 6:
			adjustPosition(moveSpeed, 0, moveSpeed);
			break;
		case 7:
			adjustPosition(-moveSpeed, 0, -moveSpeed);
			break;
		case 8:
			adjustPosition(moveSpeed, moveSpeed, 0);
			break;
		case 9:
			adjustPosition(-moveSpeed, -moveSpeed, 0);
			break;
		case 10:
			adjustPosition(0, moveSpeed, moveSpeed);
			break;
		case 11:
			adjustPosition(0, -moveSpeed, -moveSpeed);
			break;
		default:
			break;
		}
		timeMoving += delta.GetSecondsElapsed();
		delta.restart();
	}
	void SetRandomMove()
	{
		m_moveSpeed = ((float)(rand() % 100) + 10.f);
		direction = (rand() / static_cast <float> (RAND_MAX / 11));
		moveTime = (rand() / static_cast <double> (RAND_MAX / 1));
		m_moveSpeed += direction;

	}
	void SetRandomSpawn ()
	{
		double delt = delta.GetMillisecondsElapsed() * (rand() / static_cast <float> (RAND_MAX / 10));
		DirectX::XMFLOAT3 playerpos = g->m_Camera.getPosition();
		float x = (rand() % 300 + (-150));
		float y = (rand() % 30 + 15);
		float z = (rand() % 300 + (-150));

		float rando;
		//if ((rando = (rand() / static_cast <float> (RAND_MAX / 5))) <= 2.5)
		//{
		//	x = -x;
		//	z = -z;
		//}

		setPosition(x,y,z);


	}
	double moveTime = 0.f;
	double timeMoving = 0.f;
	Timer aliveTime;
	int hits = 0;
	DWORD prevDirection;
	double m_moveSpeed;
	DWORD direction;
	graphics* g;
	Timer delta;
};

