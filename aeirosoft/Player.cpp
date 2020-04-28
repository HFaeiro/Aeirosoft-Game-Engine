#include "Player.h"
#include <thread>
Player::Player(graphics* g, input* i, const std::wstring& startingGun, const std::wstring& shotSound, const std::wstring& filename, float wScale) :
	g(g), main(g, startingGun,shotSound, wScale),
	Entity(g, i, filename), i(i)
{

	walkingSoundEffect = g->CreateSound(L"Data\\Sounds\\Concrete_Shoes_Walking.wav");
	walkingSound = walkingSoundEffect->CreateInstance();
	DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight, playerWidth);
	CreateBoundingOrientedBox(playerSize);
}

void Player::Update()
{
	std::wstringstream wss;
	wss << L"Health: " << health;

	g->pSpriteBatch->Begin();
	g->pSpriteFont->DrawString(g->pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 0));
	g->pSpriteBatch->End();

	g->Begin3DScene();
	if (health <= 0)
		return;
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
				i->AdjustInputs(-recoil.x * main.stats.hipPenalty, recoil.y);
				prevAimDif = { recoil.x * main.stats.hipPenalty, -recoil.y };
			}
			recoilTimer.restart();
		}

	}
	if (isRightClick() && rCReleased)
	{
		rCReleased = false;
		if (clickTimer.GetSecondsElapsed() == 0 || clickTimer.GetSecondsElapsed() >= .2)
		{
			clickTimer.restart();
			if (aiming)
			{
				g->ChangeFOV(90.f);
			}
			else
			{
				g->ChangeFOV(60.f);
				main.SetCurrentAnimation("Aiming");
			}
			aiming = !aiming;

		}
	}
	else if(!isRightClick())
		rCReleased = true;
	if (isLeftClick())
		Shooting = false;


	viewInverse = DirectX::XMMatrixInverse(NULL, g->GetViewMatrix());
	main.UpdateWorldMatrixWithViewMatrix(viewInverse);
	main.Render();
	//playerModel.Render(g->m_TextureShader);


}

std::optional<Events*> Player::Queue()
{
	if (health <= 0)
		return this;
	if (hit) {
		health -= 20 * GetDeltaTime();
		hit = false;
	}



	float moveSpeed = g_moveSpeed;
	if (isKey(DIK_LSHIFT) && !aiming)
	{
		moveSpeed *= 2;
		running = true;
	}
	else
		running = false;


	//playerModel.UpdateWorldMatrixWithViewMatrix(viewInverse);
	if (isKey(DIK_S) || isKey(DIK_W) || isKey(DIK_D) || isKey(DIK_A)) {
		if(!aiming)
			main.SetCurrentAnimation("walking");
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
		if (!walkingSound->GetState())
		{
			walkingSound->Play();
		}
	}
	else if( !aiming)
	{
		walkingSound->Stop();
		main.SetCurrentAnimation("");
	}
	else
		walkingSound->Stop();


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
	adjustPosition(camera::movementType::up, gravity);
	if (falling)
	{
		timefalling += deltaTimer.GetMillisecondsElapsed() * .21;
	}
	else
		timefalling = 0;
	deltaTimer.restart();
	return this;
}
