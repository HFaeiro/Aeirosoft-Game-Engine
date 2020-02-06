#pragma once
#include "Events.h"
#include "model.h"
#include "Collision.h"
#include <time.h>
#include "timer.h"
#include <sstream>
class MovingAimBox : public Events , public model 

{
public:
	MovingAimBox(graphics* g): g(g) {};
	~MovingAimBox() {};

	virtual bool Initialize()
	{
		*((model*)this) = model(L"Data\\Objects\\Target\\target.obj", g);
		srand(static_cast <unsigned> (time(0)));
		SetRandomSpawn();
		//UpdateScale({ 20, 20, 20});
		delta.restart();
		aliveTime.restart();
		return true;
	}
	virtual void Update()
	{

		if (direction == prevDirection)
		{
			float rando;
			if ((rando = (rand() / static_cast <float> (RAND_MAX / 10))) >= 1)
			{
				SetRandomMove();
				if (rando >= 5)
					moveSpeed = -moveSpeed;
				Update();
				return;

			}
			moveSpeed *= (rand() / static_cast <float> (RAND_MAX * rando)) * direction;
		}
		DirectX::XMFLOAT3 pos = getPosition();
		moveSpeed = moveSpeed * delta.GetSecondsElapsed();


		if (pos.y < 10 )
		{
			direction = 2;
			//moveSpeed *= 2;
		}
		if (pos.y > 40 && direction == 2)
		{
			direction = 3;
			//moveSpeed *= 2;
		}
		if (pos.z < -100 && direction == 5)
		{
			direction = 4;
			//moveSpeed *= 2;
		}
		if (pos.z > 100 && direction == 4)
		{
			direction = 5;
			//moveSpeed *= 2;
		}
		if (pos.x < -100 && direction == 1)
		{
			direction = 0;
			//moveSpeed *= 2;
		}
		if (pos.x > 100 && direction == 0)
		{
			direction = 1;
			//moveSpeed *= 2;
		}
		if (pos.x > 100 && pos.z > 100)
		{
			direction = 7;
			//moveSpeed *= 2;
		}
		if (pos.x < -100 && pos.z < -100)
		{
			direction = 6;
			//moveSpeed *= 2;
		}
		switch (direction)
		{
		case 0:
			adjustPosition(moveSpeed, 0, 0);
			break;
		case 1:
			adjustPosition(-moveSpeed, 0, 0);
			break;
		case 2:
			adjustPosition(0,moveSpeed, 0);
			break;
		case 3:
			adjustPosition(0,-moveSpeed, 0);
			break;
		case 4:
			adjustPosition( 0, 0,moveSpeed);
			break;
		case 5:
			adjustPosition(0, 0,-moveSpeed);
			break;
		case 6:
			adjustPosition(moveSpeed,0, moveSpeed);
			break;
		case 7: 
			adjustPosition(-moveSpeed, 0, -moveSpeed);
			break;
		case 8:
			adjustPosition(moveSpeed, moveSpeed ,0);
			break;
		case 9:
			adjustPosition(-moveSpeed,-moveSpeed ,0);
			break;
		case 10:
			adjustPosition(0,moveSpeed, moveSpeed);
			break;
		case 11:
			adjustPosition(0, -moveSpeed,-moveSpeed);
			break;
		default:
			break;
		}
		delta.restart();
		Render(g->m_TextureShader);
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
		hit = false;
		srand(static_cast <unsigned> (time(0)));
		SetRandomMove();

		return this;
	}
	int GetHits()
	{
		return hits;
	}
private:
	void SetRandomMove()
	{
		moveSpeed = (rand() / static_cast <float> (RAND_MAX / 25));
		direction = (rand() / static_cast <float> (RAND_MAX / 11));

	}
	void SetRandomSpawn ()
	{
		float x = (rand() / static_cast <float> (RAND_MAX / 80));
		float y = (rand() / static_cast <float> (RAND_MAX / 20));
		float z = (rand() / static_cast <float> (RAND_MAX / 80));

		float rando;
		if ((rando = (rand() / static_cast <float> (RAND_MAX / 5))) <= 1)
		{
			x = -x;
			z = -z;
		}

		setPosition(x,y,z);


	}
	Timer aliveTime;
	int hits = 0;
	DWORD prevDirection;
	float moveSpeed;
	DWORD direction;
	graphics* g;
	Timer delta;
};

