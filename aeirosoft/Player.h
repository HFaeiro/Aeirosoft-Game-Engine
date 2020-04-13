#pragma once
#include "framework.h"
#include "Entity.h"
#include "input.h"
#include "weapon.h"
#include <sstream>

class Player : public Entity
{
public:
#ifdef _DEBUG
	void test()
	{
		double delt = deltaTimer.GetSecondsElapsed();
		float moveSpeed = .4f * delt;
		bool shift = isKey(DIK_LSHIFT);
		bool ctrl = isKey(DIK_LCONTROL);
		if (isKey(DIK_UP)) {
			if (!shift && !ctrl)
				main.adjustPosition({ 0 ,moveSpeed,0 });
			else if (shift && !ctrl)
				main.adjustRotation(0, 0, moveSpeed);
			else
				main.adjustRotation(0, moveSpeed, 0);
		}
		if (isKey(DIK_DOWN)) {
			if (!shift && !ctrl)
				main.adjustPosition({  0 , -moveSpeed ,0});
			else if (shift && !ctrl)
				main.adjustRotation(0, 0, -moveSpeed);
			else
				main.adjustRotation(0, -moveSpeed, 0);
		}
		if (isKey(DIK_LEFT))
		{
			if (!shift && !ctrl)
				main.adjustPosition({ moveSpeed,0, 0 });
			else if(shift && !ctrl)
				main.adjustRotation(moveSpeed, 0, 0 );
			else
				main.adjustPosition({ 0  ,0 , moveSpeed });
		}
		if (isKey(DIK_RIGHT))
		{
			if (!shift && !ctrl)
				main.adjustPosition({ -moveSpeed, 0 ,0 });
			else if(shift && !ctrl)
				main.adjustRotation(-moveSpeed, 0,  0  );
			else
				main.adjustPosition({ 0  ,0 , -moveSpeed });
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
#endif

	Player(graphics* g, input* i, const std::wstring& startingGun, PR hip, PR ADS,
		const std::wstring& filename, float wScale = 1.f) :
										g(g), main(g, startingGun, wScale),
										Entity(g, i, filename), hip(hip), ADS(ADS), i(i)
	{
		main.setPosition(hip.pos);
		main.setRotation(hip.rot);
		setPosition(20, playerHeight, -20);
		//playerModel.init(filename, pDevice, pContext);
	}
	~Player() {
		main.~weapon();

	};

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
		double recoilDelta = recoilTimer.GetSecondsElapsed();
		if (recoilDelta > main.stats.recoverRate)
		{
			i->AdjustInputs(prevAimDif.x * recoilDelta * 2, prevAimDif.y * recoilDelta * 2);
			recoilTimer.Stop();
		}
		Entity::_Update();
		if (isLeftClick())
		{
			if (main.shoot())
			{
				++shots;
				AddRay();
				DirectX::XMFLOAT2 recoil = main.recoil();
				DirectX::XMFLOAT2 prevAim;
				i->GetMouse(prevAim);
				if (aiming)
				{
					i->AdjustInputs(-recoil.x, recoil.y);
					prevAimDif = { recoil.x, -recoil.y };
				}
				else
				{
					i->AdjustInputs(-recoil.x * main.stats.hipPenalty, recoil.y );
					prevAimDif = { recoil.x * main.stats.hipPenalty, -recoil.y };
				}
				recoilTimer.restart();
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

		viewInverse = DirectX::XMMatrixInverse(NULL, g->GetViewMatrix());
		main.UpdateWorldMatrixWithViewMatrix(viewInverse);
		main.Render();
		//playerModel.Render(g->m_TextureShader);

		
	};
	virtual std::optional<Events*> Queue() 
	{ 

		float moveSpeed = g_moveSpeed;
		if (isKey(DIK_LSHIFT) && !aiming)
		{
			moveSpeed *= 2;
			running = true;
		}
		else
			running = false;


		//playerModel.UpdateWorldMatrixWithViewMatrix(viewInverse);
		if (isKey(DIK_W) && !isKey(DIK_D) && !isKey(DIK_A))
			adjustPosition(camera::movementType::forward, moveSpeed * GetDeltaTime());
		else if (isKey(DIK_W) && !isKey(DIK_S) && isKey(DIK_D))
		{
			adjustPosition(camera::movementType::forwardRight, moveSpeed * GetDeltaTime());
		}
		else if (isKey(DIK_W) && !isKey(DIK_D) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::forwardLeft, moveSpeed * GetDeltaTime());
		}
		else if (isKey(DIK_S) && !isKey(DIK_D) && !isKey(DIK_A))
			adjustPosition(camera::movementType::backward, moveSpeed * GetDeltaTime());

		else if (!isKey(DIK_W) && !isKey(DIK_S) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::left, moveSpeed * GetDeltaTime());

		}
		else if (!isKey(DIK_W) && !isKey(DIK_S) && isKey(DIK_D))
		{
			adjustPosition(camera::movementType::right, moveSpeed * GetDeltaTime());
		}
		else if (!isKey(DIK_W) && isKey(DIK_S) && isKey(DIK_A))
		{
			adjustPosition(camera::movementType::backLeft, moveSpeed * GetDeltaTime());
		}
		else if (!isKey(DIK_W) && isKey(DIK_S) && isKey(DIK_D))
		{
			adjustPosition(camera::movementType::backRight, moveSpeed * GetDeltaTime());
		}
		DirectX::XMFLOAT3 pos = getPosition();

		float gravity = 0.f;
		if (isKey(DIK_SPACE) && !jumping && !falling)
		{
			jumping = true;
			jumpStart = getPosition().y;
		}
		if (jumping)
		{

			if (getPosition().y - jumpStart >= jumpHeight)
				hangTimer.Start();
			else
				gravity = 120 * GetDeltaTime();

			if ((hangTimer.GetMillisecondsElapsed() * .001) >= hangTime)
			{
				jumping = false;
				falling = true;
				hangTimer.Stop();
			}

		}
		else {
			gravity = -((70.f + timefalling) * GetDeltaTime());

		}
		//adjustPosition(camera::movementType::up, gravity);
		if (falling)
		{
			timefalling += deltaTimer.GetMillisecondsElapsed() * .21;
		}
		else
			timefalling = 0;
		deltaTimer.restart();
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
	bool running = false;
	bool crouched = false;
	int shots = 0;
	float playerHeight = 100.0f;
	float jumpHeight = 40.f;
	float jumpStart = 0.f;
	float playerWidth = 10.0f;
	float playerZWidth = 15.f;
	float hangTime = .0595f;
	float g_moveSpeed = 120;
	Timer hangTimer;
	float GetDeltaTime() { return deltaTimer.GetMillisecondsElapsed() * .001f; }
	Timer deltaTimer;
	Timer recoilTimer;
	Timer clickTimer;
	bool jumping = false;
	std::vector<Events*> events;
	float health = 100.f;
	float timefalling = 0.f;
	//std::vector<weapons> equippedWeapons;
	DirectX::XMFLOAT2 prevAimDif;

	weapon main;
	input *i;
	//weapon second;
	DirectX::XMMATRIX viewInverse;

	PR hip;
	PR ADS;

};

