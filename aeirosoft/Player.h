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
#include <sstream>
class Player : public Events , public Collidable
{
public:

	void test()
	{
		double delt = deltaTimer.GetSecondsElapsed();
		float moveSpeed = 1 * delt;
		bool shift = i->isKey(DIK_LSHIFT);
		if (i->isKey(DIK_UP)) {
			if (!shift)
				main.adjustPosition({ 0,moveSpeed, 0 });
			else
				main.adjustRotation(0,0, moveSpeed);
		}
		if (i->isKey(DIK_DOWN)) {
			if (!shift)
				main.adjustPosition({ 0,-moveSpeed, 0 });
			else
				main.adjustRotation(0, 0,-moveSpeed);
		}
		if (i->isKey(DIK_LEFT))
		{
			if (!shift)
				main.adjustPosition({ moveSpeed,0, 0 });
			else
				main.adjustRotation(0, moveSpeed,0 );
		}
		if (i->isKey(DIK_RIGHT))
		{
			if (!shift)
				main.adjustPosition({ -moveSpeed, 0 ,0 });
			else
				main.adjustRotation(0, -moveSpeed, 0  );
		}

		std::wstringstream wss, wss1;
		DirectX::XMFLOAT3 pos = main.getPosition();
		DirectX::XMFLOAT3 rot = main.getRotation();

		wss << L"Position X: " << pos.x;
		wss << L" Y: " << pos.y;
		wss << L" Z: " << pos.z;

		wss1 << L"Rotation X: " << rot.x;
		wss1 << L" Y: " << rot.y;
		wss1 << L" Z: " << rot.z;

		g->pSpriteBatch->Begin();
		g->pSpriteFont->DrawString(g->pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 60));
		g->pSpriteFont->DrawString(g->pSpriteBatch.get(), wss1.str().c_str(), DirectX::XMFLOAT2(0, 80));
		g->pSpriteBatch->End();
		
		g->Begin3DScene();

	}


	Player(graphics* g, input* i, const std::wstring& startingGun, PR hip, PR ADS,
		const std::wstring& filename) :
										g(g), c(&g->m_Camera), main(g, startingGun, .75f),
										i(i), Collidable(g), hip(hip), ADS(ADS)
	{
		main.setPosition(hip.pos);
		main.setRotation(hip.rot);
		c->setPosition(0, playerHeight - 2, -5);
		//playerModel.init(filename, pDevice, pContext);
	}
	~Player() { };

	virtual bool Initialize()
	{

		deltaTimer.Start();
		DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight, playerZWidth);
		CreateBoundingOrientedBox(playerSize);
		return true;
	}
	virtual void Update()
	{
		//test();
		DirectX::XMFLOAT3 pos = c->getPosition();
		
		if (i->isKey(DIK_SPACE) && !jumping && !falling)
			jumping = true;
		if (jumping && !falling)
		{

			if (pos.y < playerHeight + 10)
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
		if (i->isLeftClick())
		{
			if (main.shoot())
			{
				++shots;
				AddRay();
			}
		}
		if (i->isRightClick())
		{
			if (clickTimer.GetSecondsElapsed() == 0 || clickTimer.GetSecondsElapsed() >= .2)
			{
				clickTimer.restart();
				if (aiming)
				{
					main.setPosition(hip.pos);
					main.setRotation(hip.rot);
				}
				else
				{
					main.setPosition(ADS.pos);
					main.setRotation(ADS.rot);
				}
				aiming = !aiming;

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
	bool aiming = false;
	int shots = 0;
	float playerHeight = 20.0f;
	float playerWidth = 5.0f;
	float playerZWidth = 2.5f;
	float hangTime = .075f;
	Timer hangTimer;
	float GetDeltaTime() { return deltaTimer.GetMillisecondsElapsed() * .001f; }
	Timer deltaTimer;
	Timer shootTimer;
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

	PR hip;
	PR ADS;

};

