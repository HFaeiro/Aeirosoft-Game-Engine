#include "MovingAimBox.h"

MovingAimBox::MovingAimBox(graphics* g) : g(g), EntityAi(g, L"Data\\Objects\\Target\\target.obj", 25) { srand(static_cast <unsigned> (time(0))); }

std::optional<Events*> MovingAimBox::Queue()
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

void MovingAimBox::UpdateMove()
{
	using namespace DirectX;
	DirectX::XMFLOAT3 pos = getPosition();
	double elap = delta.GetMillisecondsElapsed() * .000666f;
	double moveSpeed = m_moveSpeed * elap;

	DirectX::XMFLOAT3 mypos = getPosition();
	DirectX::XMFLOAT3 playerPos = g->m_Camera.getPosition();

	DirectX::XMFLOAT3 posDifference = { playerPos.x - mypos.x , playerPos.y - mypos.y,  playerPos.z - mypos.z };


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
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		if (!timeMoving)
			moveTime *= .8f;
		DirectX::XMFLOAT3 posVecToPlayer;
		DirectX::XMStoreFloat3(&posVecToPlayer, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&posDifference) * moveSpeed));
		adjustPosition(posVecToPlayer);
		break;
	default:
		break;
	}
	timeMoving += delta.GetSecondsElapsed();
	adjustRotation(timeMoving * delta.GetSecondsElapsed(), moveSpeed * delta.GetSecondsElapsed(), 1 * delta.GetSecondsElapsed());
	delta.restart();
}

void MovingAimBox::SetRandomMove()
{
	m_moveSpeed = ((float)(rand() % 1500) + 80.f);
	direction = (rand() / static_cast <float> (RAND_MAX / 20));
	moveTime = (rand() / static_cast <double> (RAND_MAX / 1));
	m_moveSpeed += direction;

}

void MovingAimBox::SetRandomSpawn()
{
	//double delt = delta.GetMillisecondsElapsed() * (rand() / static_cast <float> (RAND_MAX / 10));
	//DirectX::XMFLOAT3 playerpos = g->m_Camera.getPosition();
	float x = (rand() % 5000 + (-1500));
	float y = (rand() % 200 + 600);
	float z = (rand() % 5000 + (-1500));

	float rando;
	//if ((rando = (rand() / static_cast <float> (RAND_MAX / 5))) <= 2.5)
	//{
	//	x = -x;
	//	z = -z;
	//}

	setPosition(x, y, z);


}
