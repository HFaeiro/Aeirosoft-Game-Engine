#include "MovingAimBox.h"

MovingAimBox::MovingAimBox(graphics* g) : g(g), EntityAi(g, L"Data\\Objects\\Bully\\Bully.fbx", 25) { srand(static_cast <unsigned> (time(0))); }

std::optional<Events*> MovingAimBox::Queue()
{
	if (health)
	{
		if (hit) {
			health -= 100;
		}

		hit = false;
		if (moveTime <= timeMoving) {
			SetRandomMove();
			timeMoving = 0.f;
		}
		UpdateMove(); 
	}
	else
	{
		//if (!dead)
		//{

		//	dead = true;
		adjustPosition({ 0, -deadDelta  , 0 });
			LookAt({ 0, -deadDelta  , 0 }, true);
			resolve = false;
		//}
		g->TurnOffCulling();

		//if (isAnimActive())
			//TransformBounds(getWorld());

		g->TurnOnCulling();
		deadDelta += delta.GetSecondsElapsed();
		delta.restart();
		if (deadDelta >= 10.f)
		{
			Initialize();

		}
	}
	TransformBounds(getWorld());
	return this;
}

void MovingAimBox::UpdateMove()
{
	using namespace DirectX;
	DirectX::XMFLOAT3 pos = getPosition();
	double elap = delta.GetMillisecondsElapsed() * .001f;
	double moveSpeed = m_moveSpeed * elap;

	DirectX::XMFLOAT3 mypos = getPosition();
	DirectX::XMFLOAT3 playerPos = g->m_Camera.getPosition();

	DirectX::XMFLOAT3 posDifference = { playerPos.x - mypos.x , playerPos.y - mypos.y,  playerPos.z - mypos.z };

	DirectX::XMVECTOR toVector = {};
	switch (direction)
	{
	case 0:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3( moveSpeed, 0,0 ));
		adjustPosition(moveSpeed, 0, 0);
		break;
	case 1:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(-moveSpeed, 0, 0));
		adjustPosition(-moveSpeed, 0, 0);
		break;
	case 2:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, moveSpeed, 0));
		adjustPosition(0, moveSpeed, 0);
		break;
	case 3:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, -moveSpeed, 0));
		adjustPosition(0, -moveSpeed, 0);
		break;
	case 4:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 0, moveSpeed));
		adjustPosition(0, 0, moveSpeed);
		break;
	case 5:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 0, -moveSpeed));
		adjustPosition(0, 0, -moveSpeed);
		break;
	case 6:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(moveSpeed, 0, moveSpeed));
		adjustPosition(moveSpeed, 0, moveSpeed);
		break;
	case 7:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(-moveSpeed, 0, -moveSpeed));
		adjustPosition(-moveSpeed, 0, -moveSpeed);
		break;
	case 8:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(moveSpeed, moveSpeed, 0));
		adjustPosition(moveSpeed, moveSpeed, 0);
		break;
	case 9:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(-moveSpeed, -moveSpeed, 0));
		adjustPosition(-moveSpeed, -moveSpeed, 0);
		break;
	case 10:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, moveSpeed, moveSpeed));
		adjustPosition(0, moveSpeed, moveSpeed);
		break;
	case 11:
		toVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, -moveSpeed, -moveSpeed));
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
		DirectX::XMFLOAT3 posVecToPlayer;
		DirectX::XMStoreFloat3(&posVecToPlayer, DirectX::XMVector3NormalizeEst(DirectX::XMLoadFloat3(&posDifference)) * moveSpeed);
		adjustPosition(posVecToPlayer);
		toVector = DirectX::XMLoadFloat3(&posDifference);
		break;
	default:
		break;
	}
	timeMoving += delta.GetSecondsElapsed();
	//adjustRotation(timeMoving * delta.GetSecondsElapsed(), moveSpeed * delta.GetSecondsElapsed(), 1 * delta.GetSecondsElapsed());
	DirectX::XMFLOAT3 tof3;
	if (direction < 12) {
		DirectX::XMStoreFloat3(&tof3, toVector * m_moveSpeed);
		model::SetCurrentAnimation("flying");
	}
	else
	{
		DirectX::XMStoreFloat3(&tof3, toVector * m_moveSpeed);
		model::SetCurrentAnimation("attack");
	}
	LookAt(tof3, true);
	lastLookAt = tof3;
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
