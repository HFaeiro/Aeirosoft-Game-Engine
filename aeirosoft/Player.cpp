#include "Player.h"
#include <thread>
Player::Player(graphics* g, input* i, const std::wstring& filename, float wScale) :
	g(g), Controllable(g, i, filename), gui(g, i)
{
	Collidable::type = Collidable::eEntity;
	walkingSoundEffect = g->CreateSound(L"Data\\Sounds\\Concrete_Shoes_Walking.wav");
	walkingSound = walkingSoundEffect->CreateInstance();
	
	DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight, playerWidth);
	CreateBoundingOrientedBox(playerSize);

	RECT r;

	GetWindowRect(g->GetWindow(), &r);
	r.right = r.right - r.left;
	r.bottom = r.bottom - r.top;

	r.top = r.bottom * .5;
	r.left = -r.right * .5;

	r.bottom = -r.top;
	r.right = -r.left;

	float middleX = r.left * .5f;
	float middleY = r.bottom * .5f;

	gui.AddImage(L"Player", L"Data\\Player\\4x4pixelWhiteDot.png", { - 4,  - 4 }, { 0, 0 });
	gui.SetMainMenu(L"Player");
	gui.Initialize();
}

void Player::Update()
{

	std::stringstream ss = getItems();
	g->pSpriteBatch->Begin();
	g->pSpriteFont->DrawString(g->pSpriteBatch.get(), ss.str().c_str(), DirectX::XMFLOAT2(0, 0));
	g->pSpriteBatch->End();

	if (health <= 0)
		return;

	Controllable::Update();
	float milis = deltaTimer.GetMillisecondsElapsed();


	PulseAndCheckFaceRay();
	UseInHand();

}
void Player::PulseAndCheckFaceRay()
{
	double actionDelta = actionTimer.GetMillisecondsElapsed();
	if(actionDelta > 250 || actionDelta <=0)
		AddHitRay();
	if (rayCollidedWith.size())
	{
		gui.Update();

		if (actionDelta > .500f || actionDelta <= 0)
			//interact key
			if (isKey(DIK_F))
			{
				for (const auto& object : rayCollidedWith)
				{
					if (const auto optional = object.collided->OnCollision())//fuccckkakskdafswamidoinghere?!thefuckkkkduddee//still not right. need to change with interactable.h
					{
						//pickup item
						Item* newItem = (Item*)*optional;
						if (addItem(newItem, 1))
						{
							//check if item is equipable
							if (newItem->itemInfo.equipable)
								//check if hands are empty
								if (inHand == nullptr)
								{
									inHand = newItem;
								}

							//cleanup
							rayCollidedWith.clear();

						}
					}
				}
				actionTimer.restart();

			}
	}
}
void Player::UseInHand()
{
	if (inHand != nullptr)
	{
		if (isKey(DIK_Y))
			inHand->SetCurrentAnimation("Wave");
		viewInverse = DirectX::XMMatrixInverse(NULL, g->GetViewMatrix());
		inHand->UpdateWorldMatrixWithViewMatrix(viewInverse);
		inHand->Update();
		if (isLeftClick())
		{
			if (((gun*)(inHand))->stats.automatic || lCReleased)
				if (inHand->LeftClick())
				{
					DirectX::XMFLOAT2 recoil = ((gun*)(inHand))->recoil();
					DirectX::XMFLOAT2 prevAim;
					i->GetMouse(prevAim);
					if (aiming)
					{
						i->AdjustInputs(-recoil.y, recoil.x);
						prevAimDif = { recoil.y, -recoil.x };
					}
					else
					{
						i->AdjustInputs(-recoil.y * ((gun*)(inHand))->stats.hipPenalty, recoil.x);
						prevAimDif = { recoil.y * ((gun*)(inHand))->stats.hipPenalty, -recoil.x };
					}
					recoilTimer.restart();
				}
			lCReleased = false;
		}
		else
			lCReleased = true;

		if (isRightClick() && rCReleased)
		{
			rCReleased = false;
			if (clickTimer.GetSecondsElapsed() == 0 || clickTimer.GetSecondsElapsed() >= .2)
			{
				clickTimer.restart();
				if (aiming)
				{
					g->ChangeFOV(65.f);
				}
				else
				{
					g->ChangeFOV(95);
					inHand->SetCurrentAnimation("Aiming");
				}
				aiming = !aiming;
				if (!aiming)
					inHand->SetCurrentAnimation("");
			}
		}

		else if (!isRightClick())
			rCReleased = true;


		if (isKey(DIK_2))
		{
			if (inventory.secondary)
			{
				inHand = inventory.secondary;
			}
		}
		else if (isKey(DIK_1))
		{
			if (inventory.primary)
			{
				inHand = inventory.primary;
			}
		}

	}
}
std::optional<Events*> Player::Queue()
{
	rayCollidedWith.clear();
	if (health <= 0)
		return this;
	if (hit) {
		health -= 20 * GetDeltaTime();
		hit = false;
	}

	if (isKey(DIK_C))
	{
		double actionDelta = actionTimer.GetMillisecondsElapsed();
		if (actionDelta > 250 || actionDelta <= 0)
		{
			DirectX::XMFLOAT3 playerSize = DirectX::XMFLOAT3(playerWidth, playerHeight + (crouched ? 0 : -20), playerWidth);
			CreateBoundingOrientedBox(playerSize);
			crouched = !crouched;
			actionTimer.restart();
		}
	}

	float moveSpeed = g_moveSpeed;
	if (isKey(DIK_LSHIFT) && !aiming && !crouched)
	{
		moveSpeed *= 2;
		running = true;
		walkingSound->SetPitch(.9f);
		walkingSound->SetVolume(5);
	}
	else
	{

		running = false;
		if (crouched) {
			moveSpeed *= .5;
			walkingSound->SetPitch(-.5);
			walkingSound->SetVolume(.5);
		}
		else
		{
			walkingSound->SetPitch(0);
			walkingSound->SetVolume(1);
		}
	}
	//playerModel.UpdateWorldMatrixWithViewMatrix(viewInverse);
	
	if (isKey(DIK_S) || isKey(DIK_W) || isKey(DIK_D) || isKey(DIK_A)) {
		//if(!aiming)
			//main.SetCurrentAnimation("walking");
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
		if (inHand && !aiming)
			inHand->SetCurrentAnimation("walking");
	}
	//else /*if( !aiming)*/
	//{
	//	walkingSound->Stop();

	//}
	else
		walkingSound->Stop();
	float fov = g->GetFovDegrees();
	if (aiming)
	{
		if (fov > 60)
		{
			float change = (.25 * deltaTimer.GetMillisecondsElapsed());
			if(fov - change < 60)
				g->ChangeFOV(60);
			else
				g->ChangeFOV(fov - change);
		}
	}
	else
		if (fov < 100)
		{
			float change = (.25 * deltaTimer.GetMillisecondsElapsed());
			if (fov + change > 100)
				g->ChangeFOV(100);
			else
				g->ChangeFOV(fov + change);
		}

	DirectX::XMFLOAT3 pos = getPosition();

	float gravity = 0.f;
	if (isKey(DIK_SPACE) && !jumping && !falling && !crouched)
	{
		jumping = true;
		jumpStart = getPosition().y;
	}
	if (jumping)
	{
		walkingSound->Stop();
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
		if(timefalling)
			walkingSound->Stop();
		gravity = -((70.f + timefalling) * GetDeltaTime());

	}

	adjustPosition(camera::movementType::up, gravity);


	//adjustPosition(camera::movementType::up, gravity);
	if (falling)
	{
		timefalling += deltaTimer.GetMillisecondsElapsed() * .21;
	}
	else
		timefalling = 0;
	deltaTimer.restart();
	return this;
}


