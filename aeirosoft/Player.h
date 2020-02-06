#pragma once
#include "framework.h"
#include "model.h"
#include "camera.h"
#include "Events.h"
#include "input.h"
#include "weapon.h"
#include "graphics.h"
#include "Collision.h"
#include <DirectXCollision.h>

class Player : public Events , public Collidable
{
public:

	Player(graphics* g, input* i, const std::wstring& startingGun, DirectX::XMFLOAT3 gunPos, DirectX::XMFLOAT3 gunRot,
		const std::wstring& filename) :
										g(g), c(&g->m_Camera), main(g, startingGun),
										i(i), Collidable(g)
	{
		main.setPosition(gunPos);
		main.setRotation(gunRot);
		c->setPosition(0, 12.f, -5);
		//playerModel.init(filename, pDevice, pContext);
	}
	~Player() { };

	virtual bool Initialize()
	{

		deltaTimer.Start();
		DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight, playerZWidth);
		CreateBoundingBox(playerSize);
		return true;
	}
	virtual void Update()
	{
		DirectX::XMFLOAT3 pos = c->getPosition();
		
		if (i->isKey(DIK_SPACE) && !jumping && !falling)
			jumping = true;
		if (jumping && !falling)
		{

			if (pos.y < 20)
				c->adjustPosition(camera::movementType::up, 45 * GetDeltaTime());
			else
			{
				
				if (hangTimer.GetMillisecondsElapsed() == 0)
					hangTimer.Start();
				if ((hangTimer.GetMillisecondsElapsed() * .001) >= hangTime)
				{
					jumping = false;
					falling = true;
					hangTimer.Stop();
				}
			}
		}
		else if (pos.y > playerHeight - 2)
		{

			c->adjustPosition(camera::movementType::up, -55 * GetDeltaTime());

		}
		else
			falling = false;
		if (i->isLeftClick() && !Shooting)
		{
			Shooting = true;
			if (clickTimer.GetSecondsElapsed() == 0 || clickTimer.GetSecondsElapsed() >= .2)
			{
				clickTimer.restart();
				AddRay();
				shots += 1;
			}
		}
		else if (!i->isLeftClick())
			Shooting = false;

		main.Render();
		//DrawBoundingBox();
		//playerModel.Render(g->m_TextureShader);

		deltaTimer.restart();
	};
	virtual std::optional<Events*> Queue() 
	{ 
		
		viewInverse = DirectX::XMMatrixInverse(NULL, c->getViewMatrix());
		main.UpdateWorldMatrixWithViewMatrix(viewInverse);
		//playerModel.UpdateWorldMatrixWithViewMatrix(viewInverse);
		TransformBounds(getWorldAtViewMatrix());

		return this;
	};

	void adjustHealth(float adj) { this->health += adj; }


	int GetShots() {
		return shots;
	}



private:
	DirectX::XMMATRIX getWorldAtViewMatrix()
	{
		DirectX::XMFLOAT3 rot = c->getRotation();
		DirectX::XMFLOAT3 pos = c->getPosition();
		return  DirectX::XMMatrixRotationRollPitchYaw(0, rot.y, 0) * DirectX::XMMatrixTranslation(pos.x, pos.y - playerHeight + 2, pos.z);
	}
	
	graphics* g;
	bool Shooting = false;
	int shots = 0;
	float playerHeight = 14.0f;
	float playerWidth = 5.0f;
	float playerZWidth = 2.5f;
	float hangTime = .075f;
	Timer hangTimer;
	float GetDeltaTime() { return deltaTimer.GetMillisecondsElapsed() * .001f; }
	Timer deltaTimer;
	Timer clickTimer;
	bool jumping = false;
	bool falling = false;
	std::vector<Events*> events;
	camera* c;
	float health = 100.f;
	model playerModel;
	//std::vector<weapons> equippedWeapons;
	weapon main;
	input *i;
	//weapon second;
	DirectX::XMMATRIX viewInverse;



};

