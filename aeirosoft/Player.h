#pragma once
#include "framework.h"
#include "Entity.h"
#include "input.h"
#include "weapon.h"
#include <sstream>

class Player : public Events , public Entity
{
public:

	void test()
	{
		double delt = deltaTimer.GetSecondsElapsed();
		float moveSpeed = 1 * delt;
		bool shift = isKey(DIK_LSHIFT);
		if (isKey(DIK_UP)) {
			if (!shift)
				main.adjustPosition({ 0,moveSpeed, 0 });
			else
				main.adjustRotation(0,0, moveSpeed);
		}
		if (isKey(DIK_DOWN)) {
			if (!shift)
				main.adjustPosition({ 0,-moveSpeed, 0 });
			else
				main.adjustRotation(0, 0,-moveSpeed);
		}
		if (isKey(DIK_LEFT))
		{
			if (!shift)
				main.adjustPosition({ moveSpeed,0, 0 });
			else
				main.adjustRotation(0, moveSpeed,0 );
		}
		if (isKey(DIK_RIGHT))
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
										g(g), main(g, startingGun, .75f),
										Entity(g, i, filename), hip(hip), ADS(ADS)
	{
		main.setPosition(hip.pos);
		main.setRotation(hip.rot);
		setPosition(95, playerHeight, 95);
		//playerModel.init(filename, pDevice, pContext);
	}
	~Player() { };

	virtual bool Initialize()
	{

		deltaTimer.Start();
		DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight, playerWidth);
		CreateBoundingOrientedBox(playerSize);
		return true;
	}
	virtual void Update()
	{
#ifdef _DEBUG
		test();
#endif

		Entity::Update();
		if (isLeftClick())
		{
			if (main.shoot())
			{
				++shots;
				AddRay();
			}
		}
		if (isRightClick())
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
		else if (isLeftClick())
			Shooting = false;

		viewInverse = DirectX::XMMatrixInverse(NULL, getViewMatrix());
		main.UpdateWorldMatrixWithViewMatrix(viewInverse);
		main.Render();
		//DrawBoundingBox();
		//playerModel.Render(g->m_TextureShader);

		deltaTimer.restart();
	};
	virtual std::optional<Events*> Queue() 
	{ 
		

		//playerModel.UpdateWorldMatrixWithViewMatrix(viewInverse);
		if (isKey(DIK_W) && !isKey(DIK_S) && !isKey(DIK_D) && !isKey(DIK_A))
			adjustPosition(camera::movementType::forward, moveSpeed * GetDeltaTime());
		else if (!isKey(DIK_W) && isKey(DIK_S) && !isKey(DIK_D) && !isKey(DIK_A))
			adjustPosition(camera::movementType::backward, moveSpeed * GetDeltaTime());

		else if (!isKey(DIK_W) && !isKey(DIK_S) && !isKey(DIK_D) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::left, moveSpeed * GetDeltaTime());

		}
		else if (!isKey(DIK_W) && !isKey(DIK_S) && isKey(DIK_D) && !isKey(DIK_A))
		{
			adjustPosition(camera::movementType::right, moveSpeed * GetDeltaTime());
		}
		else if (isKey(DIK_W) && !isKey(DIK_S) && isKey(DIK_D) && !isKey(DIK_A))
		{
			adjustPosition(camera::movementType::forwardRight, moveSpeed * GetDeltaTime());
		}
		else if (isKey(DIK_W) && !isKey(DIK_S) && !isKey(DIK_D) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::forwardLeft, moveSpeed * GetDeltaTime());
		}
		else if (!isKey(DIK_W) && isKey(DIK_S) && !isKey(DIK_D) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::backLeft, moveSpeed * GetDeltaTime());
		}
		else if (!isKey(DIK_W) && isKey(DIK_S) && isKey(DIK_D) && !isKey(DIK_A))
		{
			adjustPosition(camera::movementType::backRight, moveSpeed * GetDeltaTime());
		}
		DirectX::XMFLOAT3 pos = getPosition();

		/*if (isKey(DIK_SPACE) && !jumping && !falling)
			jumping = true;
		if (jumping && !falling)
		{

			if (pos.y < playerHeight + 10)
				adjustPosition(camera::movementType::up, 45 * GetDeltaTime());
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
		else if (pos.y > playerHeight)
		{

			adjustPosition(camera::movementType::up, -55 * GetDeltaTime());

		}
		else
			falling = false;*/


		return this;
	};

	void adjustHealth(float adj) { this->health += adj; }


	int GetShots() {
		return shots;
	}



private:

	
	graphics* g;
	bool Shooting = false;
	bool aiming = false;
	int shots = 0;
	float playerHeight = 20.0f;
	float playerWidth = 5.0f;
	float playerZWidth = 2.5f;
	float hangTime = .075f;
	float moveSpeed = 100;
	Timer hangTimer;
	float GetDeltaTime() { return deltaTimer.GetMillisecondsElapsed() * .001f; }
	Timer deltaTimer;
	Timer shootTimer;
	Timer clickTimer;
	bool jumping = false;
	bool falling = false;
	std::vector<Events*> events;
	float health = 100.f;
	//std::vector<weapons> equippedWeapons;

	weapon main;

	//weapon second;
	DirectX::XMMATRIX viewInverse;

	PR hip;
	PR ADS;

};

