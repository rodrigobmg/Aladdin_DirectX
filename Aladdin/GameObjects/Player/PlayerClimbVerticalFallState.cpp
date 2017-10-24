#include "PlayerClimbVerticalFallState.h"
#include "Player.h"

PlayerClimbVerticalFallState::PlayerClimbVerticalFallState()
{
}

PlayerClimbVerticalFallState::PlayerClimbVerticalFallState(Player * player) : PlayerState(player, PlayerState::StateName::ClimbVerticalFall)
{
	_animation = new Animation(ResourceManager::GetInstance()->GetAnimationXMLAladdin(), "ClimbVerticalFall", ResourceManager::GetInstance()->GetTextureAladdin(), 0.8f);
	_animation->FlipVertical(!_player->IsRight());

	_animation->SetPosition(_player->GetPosition());
	_player->SetWidth(_animation->GetWidth());
	_player->SetHeight(_animation->GetHeight());

	_player->SetAccelerationY(_player->GetMass());
}


PlayerClimbVerticalFallState::~PlayerClimbVerticalFallState()
{
}

void PlayerClimbVerticalFallState::Update(float deltaTime)
{
	PlayerState::Update(deltaTime);

	if (_animation->IsFinish())
	{
		_player->SetState(new PlayerFallState(_player));
		return;
	}
}

void PlayerClimbVerticalFallState::OnCollision(GameObject * target, GameCollision::SideCollisions side)
{
	PlayerState::OnCollision(target, side);

	if (target->GetTag() == GameObject::GameObjectType::Rope)
	{
		float diffX = abs(target->GetPosition().x - _player->GetPosition().x);
		if (diffX < 18 && _player->GetBound().bottom <= target->GetBound().bottom && _player->GetBound().top >= target->GetBound().top)
		{
			_player->SetState(new PlayerClimbVerticalState(_player, target));
		}
	}

	if (target->GetTag() == GameObject::GameObjectType::HorizontalBar)
	{
		float diffTop = abs(target->GetBound().top - _player->GetBound().top);
		if (diffTop < 18
			&& _player->GetPosition().x >= target->GetBound().left
			&& _player->GetPosition().x <= target->GetBound().right)
			_player->SetState(new PlayerClimbHorizontalIdleState(_player, target));
	}
}
