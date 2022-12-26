#pragma once
#include "framework.h"
#include "Controllable.h"
#include "input.h"
#include "inventory.h"
#include <sstream>
#include "gun.h"
#include "Gui.h"
class Player : public Controllable , public Inventory
{
public:

	Player(graphics* g, input* i, /*const std::wstring& startingGun, const std::wstring &shotSound,*/
		const std::wstring& filename, float wScale = 1.f);
	~Player() {
		walkingSound.release();
		walkingSoundEffect.release();

	};

	virtual bool Initialize() override
	{
		deltaTimer.restart();
		Boundings[0].vAdjustments.clear();
		health = 100;
		SetRandomSpawn();
		TransformBounds(getWorldAtViewMatrix());
		falling = false;
		bool rCReleased = true;
		bool Shooting = false;
		bool aiming = false;
		bool running = false;
		bool crouched = false;
		int shots = 0;
		timefalling = 0.f;

		return true;
	}
	virtual void Update() override;
	virtual std::optional<Events*> Queue() override;

	void adjustHealth(float adj) { this->health += adj; }


	int GetShots() {
		return shots;
	}
private:
	void PulseAndCheckFaceRay();
	void UseInHand();
	void SetRandomSpawn()
	{
		float x = (rand() % 1200 + (-800));
		float y = playerHeight;
		float z = (rand() % 1200 + (-800));
		setPosition(x, y, z);
	}
	
	graphics* g;
	Gui gui;
	
	bool rCReleased = true;
	bool lCReleased = true;
	bool crouched = false;
	//bool Shooting = false;
	bool aiming = false;
	bool running = false;
	int shots = 0;
	float playerHeight = 50.0f;
	float jumpHeight = 40.f;
	float jumpStart = 0.f;
	float playerWidth = 25.0f;
	float playerZWidth = 25.f;
	float hangTime = .0595f;
	float g_moveSpeed = 120;
	float g_strafeSpeed = 80;
	Timer hangTimer;
	float GetDeltaTime() { return deltaTimer.GetMillisecondsElapsed() * .001f; }
	Timer deltaTimer;
	Timer recoilTimer;
	Timer actionTimer;
	Timer clickTimer;
	bool jumping = false;
	std::vector<Events*> events;
	float health = 100.f;
	float timefalling = 0.f;
	//std::vector<weapons> equippedWeapons;
	DirectX::XMFLOAT2 prevAimDif;


	
	//weapon main;
	Item* inHand;
	//weapon second;
	DirectX::XMMATRIX viewInverse;

	std::unique_ptr<DirectX::SoundEffect> walkingSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> walkingSound;
};

