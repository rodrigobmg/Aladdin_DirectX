#include "Player.h"
#include "../Enemies/Enemy.h"

Player::Player():GameObject(GameObject::GameObjectType::Players)
{
	_mass = 15;
	_speed = 25;
	_jumpForce = 45;
	_jumpRunForce = 48;

	_isRight = true;
	_isGround = false;
	_allowMoveLeft = _allowMoveRight = true;

	_health = 100;
	_damage = 50;
	_numAppleWeapon = 10;

	_state = new PlayerFallState(this);
}

Player::~Player()
{
	delete _state;
}

void Player::Update(float deltaTime)
{
	if (_isGround && _velocity.y>0)//change state
	{
		_acceleration.y = 0;
		_velocity.y = 0;
	}

	//face to right or left
	if (_input->IsKeyPressed(DIK_LEFT))
		_isRight = false;
	else if(_input->IsKeyPressed(DIK_RIGHT))
		_isRight = true;

	//move
	if (_input->IsKeyPressed(DIK_LEFT) && _allowMoveLeft)
		_velocity.x = -1 * _speed;
	else if (_input->IsKeyPressed(DIK_RIGHT) && _allowMoveRight)
		_velocity.x = _speed;
	else
		_velocity.x = 0;


	GameObject::Update(deltaTime);
	_state->Update(deltaTime);
}

void Player::Draw(Camera * camera)
{
	Animation *animation = _state->GetAnimation();
	if (animation != NULL)
		animation->Draw(camera);

	//debug
	/*
	D3DXVECTOR3 newPosition = camera->ConvertPosition(_position.x, _position.y);

	RECT rect;
	rect.left = newPosition.x - _width / 2;
	rect.top = newPosition.y - _height / 2;
	rect.right = newPosition.x + _width / 2;
	rect.bottom = newPosition.y + _height / 2;

	Graphics::GetInstance()->GetDevice()->ColorFill(Graphics::GetInstance()->GetBackBuffer(), &rect, D3DCOLOR_XRGB(0, 255, 0));
	*/
}

void Player::OnCollision(GameObject * target, GameCollision::SideCollisions side)
{
	if (target->GetTag() == GameObject::GameObjectType::Apple)
	{
		_numAppleWeapon++;
	}

	if (target->GetTag() == GameObject::GameObjectType::Enemies)
	{
		Enemy *enemy = dynamic_cast<Enemy*>(target);
		if (enemy->GetState()->GetName() == EnemyState::StateName::Attack && !enemy->GetState()->IsAttackedPlayer())
		{
			SetHealth(_health - enemy->GetDamage());
			enemy->GetState()->SetIsAttackedPlayer(true);
		}
	}

	if (target->GetTag() == GameObject::GameObjectType::Weapons)
	{
		Weapon* weapon = dynamic_cast<Weapon*>(target);
		if (weapon->GetWeaponType() == Weapon::WeaponType::EnemiesWeapons)
		{
			SetHealth(_health - weapon->GetDamage());
		}
	}

	_state->OnCollision(target, side);
}

PlayerState * Player::GetState()
{
	return _state;
}

void Player::SetState(PlayerState * state, bool fixFootPosition)
{
	if (fixFootPosition)
	{
		//make sure player's foot at the same position when change state. because each state has own height
		int newHeight = state->GetAnimation()->GetHeight();
		int currentHeight = _state->GetAnimation()->GetHeight();
		float diff = (currentHeight - newHeight) / 2.0;
		_position.y += diff;
	}

	//change state
	delete _state;
	_state = state;
}

bool Player::IsRight()
{
	return _isRight;
}

bool Player::IsGround()
{
	return _isGround;
}

void Player::SetIsGround(bool isGround)
{
	_isGround = isGround;
}

bool Player::IsAllowMoveLeft()
{
	return _allowMoveLeft;
}

bool Player::IsAllowMoveRight()
{
	return _allowMoveRight;
}

void Player::AllowMoveLeft(bool allow)
{
	_allowMoveLeft = allow;
}

void Player::AllowMoveRight(bool allow)
{
	_allowMoveRight = allow;
}

float Player::GetSpeed()
{
	return _speed;
}

void Player::SetSpeed(float speed)
{
	_speed = speed;
}

float Player::GetJumpForce()
{
	return _jumpForce;
}

void Player::SetJumpForce(float force)
{
	_jumpForce = force;
}

float Player::GetJumpRunForce()
{
	return _jumpRunForce;
}

void Player::SetJumpRunForce(float force)
{
	_jumpRunForce = force;
}

int Player::GetHealth()
{
	return _health;
}

void Player::SetHealth(int newHealth)
{
	_health = newHealth;

	if (_health <= 0)
	{
		SetState(new PlayerDeathState(this));
	}
}

int Player::GetDamage()
{
	return _damage;
}

void Player::SetDamage(int newDamage)
{
	_damage = newDamage;
}

int Player::GetNumAppleWeapon()
{
	return _numAppleWeapon;
}

void Player::SetNumAppleWeapon(int value)
{
	_numAppleWeapon = value;
}
