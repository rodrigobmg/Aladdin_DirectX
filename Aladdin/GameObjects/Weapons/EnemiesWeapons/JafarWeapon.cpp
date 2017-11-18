#include "JafarWeapon.h"

JafarWeapon::JafarWeapon(int damage) :Weapon(Weapon::WeaponType::EnemiesWeapons, damage)
{
	_animationFly = new Animation(ResourceManager::GetInstance()->GetAnimationXMLJafarWeapon1(), "fly", ResourceManager::GetInstance()->GetTextureBossJafar(), true, 0.4f);
	_animationExploide = new Animation(ResourceManager::GetInstance()->GetAnimationXMLAppleWeapon(), "Explode", ResourceManager::GetInstance()->GetTextureAladdin(), true, 0.4f);

	//_animationExploide = new Animation(ResourceManager::GetInstance()->GetAnimationXMLExplosions(), "Explosions", ResourceManager::GetInstance()->GetTextureExplosions(), true, 0.4f);
	
	_currentAnimation = _animationFly;
	_width = _currentAnimation->GetWidth();
	_height = _currentAnimation->GetHeight();

	_velocity.x = 1;
	_velocity.y = 1;

	_mass = 0;
	_acceleration.y = _mass;
}

JafarWeapon::~JafarWeapon()
{
	delete _animationFly;
	delete _animationExploide;
}

void JafarWeapon::Draw(Camera* camera)
{
	if (_isVisible)
		_currentAnimation->Draw(camera);
}

void JafarWeapon::Update(float deltatime)
{
	GameObject::Update(deltatime);

	_currentAnimation->SetPosition(_position);
	_currentAnimation->Update(deltatime);
	SetPosition(_currentAnimation->GetPosition());

	_width = _currentAnimation->GetWidth();
	_height = _currentAnimation->GetHeight();
}

void JafarWeapon::OnCollision(GameObject* target, GameCollision::SideCollisions side)
{
	if (target->GetTag() != GameObject::GameObjectType::Enemies)
	{
		_isVisible = false;
		_acceleration.y = 0;
		_velocity.x = _velocity.y = 0;
	}

	if (target->GetTag() != GameObject::GameObjectType::Enemies && target->GetTag() == GameObject::GameObjectType::Players)
	{
		_currentAnimation = _animationExploide;
		_width = _currentAnimation->GetWidth();
		_height = _currentAnimation->GetHeight();

		_acceleration.y = 0;
		_velocity.x = _velocity.y = 0;

	}
}