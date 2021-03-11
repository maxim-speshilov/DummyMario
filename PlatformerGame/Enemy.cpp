#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const TextureHolder& textures) :
	Entity(SceneObject::Type::kEnemy, textures) {
	setSpeed(sf::Vector2f(42.f, 0.f));
	setSize(16, 16);
}

Enemy::Enemy(SceneObject::Type type, const TextureHolder& textures) :
	Entity(SceneObject::Type::kEnemy, textures),
	hp_(1.f),
	destruction_dt_(0.f) {

	assert(type == SceneObject::Type::kEnemy);
	animation_player_.set(Animation::ID::kRunning);
	
	type_ = type;
	state_ = entities::State::kRunning;
	direction_ = entities::MoveDirection::kRight;
	rect_.setSize(sf::Vector2f(16, 16));
	rect_.setTexture(&textures.get(Textures::kEnemy));
	rect_.setTextureRect(sf::IntRect(0, 25, 16, 16));
	setSpeed(sf::Vector2f(42.f, 0.f));
}

Enemy::Enemy() {
}


void Enemy::update(float dt, CommandQueue& commands) {
	if (isDestroyed())
		setState(entities::State::kDead);

	if (state_ != entities::State::kDead) {
		rect_.move(speed_ * dt);
	} else  {
		if (!is_size_changed_) {
			is_size_changed_ = true;
			rect_.move(0.f, 8.f);
			rect_.scale(1.f, 0.5f);
		}

		destruction_dt_ += dt;
	}
	
	switch (state_) {
	case entities::State::kRunning:
		animation_player_.set(Animation::ID::kRunning);
		break;
	case entities::State::kDead:
		animation_player_.set(Animation::ID::kDead);
		break;
	default:
		break;
	}

	animation_player_.update(rect_, dt);
}

void Enemy::damage(float hp) {
	hp_ -= hp_;
}

bool Enemy::isDestroyed() const {
	return hp_ <= 0.f;
}

bool Enemy::isMarkedForRemoval() const {
	return isDestroyed() && destruction_dt_ >= 0.5f;
}

unsigned int Enemy::getCategory() const {
	return (unsigned int)category::Type::Enemy;
}
