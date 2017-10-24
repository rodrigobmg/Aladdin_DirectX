#include "PlayerClimbVerticalState.h"
#include "Player.h"


PlayerClimbVerticalState::PlayerClimbVerticalState()
{
}

PlayerClimbVerticalState::PlayerClimbVerticalState(Player * player, GameObject *rope) : PlayerState(player, PlayerState::StateName::ClimbVertical)
{
	_rope = rope;
	_player->SetPositionX(_rope->GetPosition().x);

	_animation = new Animation(ResourceManager::GetInstance()->GetAladdinAnimationXML(), "ClimbVertical", ResourceManager::GetInstance()->GetAladdinTexture(), 0.4f);
	_animation->FlipVertical(!_player->IsRight());
	_animation->Pause();

	_animation->SetPosition(_player->GetPosition());
	_player->SetWidth(_animation->GetWidth());
	_player->SetHeight(_animation->GetHeight());

	_player->SetAccelerationY(0);
	_player->SetVelocityY(0);

	_player->AllowMoveLeft(false);
	_player->AllowMoveRight(false);
}


PlayerClimbVerticalState::~PlayerClimbVerticalState()
{
}

void PlayerClimbVerticalState::Update(float deltaTime)
{
	PlayerState::Update(deltaTime);

	if (Input::GetInstance()->IsKeyPressed(DIK_UP) && _player->GetBound().top > _rope->GetBound().top)
	{
		_animation->Reverse(false);
		_animation->Play();
		_player->SetVelocityY(-10);
	}
	else if (Input::GetInstance()->IsKeyPressed(DIK_DOWN))
	{
		_animation->Reverse(true);
		_animation->Play();
		_player->SetVelocityY(10);
	}
	else
	{
		_animation->Pause();
		_player->SetVelocityY(0);
	}

	//change state
	if (Input::GetInstance()->IsKeyPressed(DIK_S))
	{
		_player->SetState(new PlayerClimbAttackState(_player, _rope, PlayerState::StateName::ClimbVertical));
		return;
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_D))
	{
		_player->SetState(new PlayerClimbVerticalJumpState(_player));
		return;
	}
	if (_player->GetBound().bottom > _rope->GetBound().bottom)
	{
		_player->SetState(new PlayerFallState(_player));
		return;
	}
}