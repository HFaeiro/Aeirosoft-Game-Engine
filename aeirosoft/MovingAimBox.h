#pragma once
#include "Events.h"
#include "tmpEntity.h"
#include <time.h>
#include "timer.h"
#include <sstream>
class MovingAimBox : public Events , public EntityAi

{
public:
	MovingAimBox(graphics* g): g(g) ,EntityAi(g, L"Data\\Objects\\Target\\target.obj", 5) { srand(static_cast <unsigned> (time(0))); };
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
		//if (aliveTime.GetSecondsElapsed() >= 25) {
		//	Initialize();
			
		//}
		if (collision) {
			revertWorld();
			if (prevDirection == -1)
			{
				Initialize();
				return {};
			}
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
		double elap = delta.GetMillisecondsElapsed() * .000666f;
		double moveSpeed = m_moveSpeed * elap;

		DirectX::XMFLOAT3 mypos = getPosition();
		DirectX::XMFLOAT3 playerPos = g->m_Camera.getPosition();

		DirectX::XMFLOAT3 posDifference = {mypos.x - playerPos.x, mypos.y - playerPos.y, mypos.z - playerPos.z };


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
		case 12:
			moveTime *= .8f;
			moveSpeed *= .5f;
			adjustPosition(-posDifference.x * moveSpeed, -posDifference.y * moveSpeed, -posDifference.z * moveSpeed);
			break;
		default:
			break;
		}
		timeMoving += delta.GetSecondsElapsed();
		adjustRotation(timeMoving * delta.GetSecondsElapsed(), moveSpeed * delta.GetSecondsElapsed(), 1 * delta.GetSecondsElapsed());
		delta.restart();
	}
	void SetRandomMove()
	{
		m_moveSpeed = ((float)(rand() % 100) + 10.f);
		direction = (rand() / static_cast <float> (RAND_MAX / 13));
		moveTime = (rand() / static_cast <double> (RAND_MAX / 1));
		m_moveSpeed += direction;

	}
	void SetRandomSpawn()
	{
		double delt = delta.GetMillisecondsElapsed() * (rand() / static_cast <float> (RAND_MAX / 10));
		DirectX::XMFLOAT3 playerpos = g->m_Camera.getPosition();
		float x = (rand() % 300 + (-150));
		float y = (rand() % 30 + playerpos.y);
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
	DWORD prevDirection = -1;
	double m_moveSpeed;
	DWORD direction;
	graphics* g;
	Timer delta;
};

