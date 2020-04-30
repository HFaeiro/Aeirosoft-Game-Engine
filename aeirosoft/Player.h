#pragma once
#include "framework.h"
#include "Entity.h"
#include "input.h"
#include "weapon.h"
#include <sstream>

class Player : public Entity
{
public:

	Player(graphics* g, input* i, const std::wstring& startingGun, const std::wstring &shotSound,
		const std::wstring& filename, float wScale = 1.f);
	~Player() {
		main.~weapon();
		walkingSound.release();
		walkingSoundEffect.release();

	};

	virtual bool Initialize()
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
	virtual void Update();
	virtual std::optional<Events*> Queue();;

	void adjustHealth(float adj) { this->health += adj; }


	int GetShots() {
		return shots;
	}



private:
	void SetRandomSpawn()
	{
		float x = (rand() % 1200 + (-800));
		float y = playerHeight;
		float z = (rand() % 1200 + (-800));
		setPosition(x, y, z);
	}
	
	graphics* g;
	bool rCReleased = true;
	bool Shooting = false;
	bool aiming = false;
	bool running = false;
	bool crouched = false;
	int shots = 0;
	float playerHeight = 50.0f;
	float jumpHeight = 40.f;
	float jumpStart = 0.f;
	float playerWidth = 25.0f;
	float playerZWidth = 25.f;
	float hangTime = .0595f;
	float g_moveSpeed = 1200;
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

	std::unique_ptr<DirectX::SoundEffect> walkingSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> walkingSound;
};

