#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Scene& level, sf::Vector2f position, int width, int height, entities::MoveDirection direction_) :
	Entity(level, position, width, height) {
	state_ = entities::State::kRunning;
	type_ = entities::Type::kEnemy;
	this->direction_ = direction_;

	if (direction_ == entities::MoveDirection::kRight)
		speed_ = sf::Vector2f(0.05, 0);
	else
		speed_ = sf::Vector2f(-0.05, 0);
}

Enemy::Enemy(Scene &level, const TextureHolder &textures, sf::Vector2f position, int width, int height, entities::MoveDirection direction_) :
	Entity(level, entities::Type::kEnemy, textures, position, width, height) {
	state_ = entities::State::kRunning;
	this->direction_ = direction_;

	if (direction_ == entities::MoveDirection::kRight)
		speed_ = sf::Vector2f(0.05, 0);
	else
		speed_ = sf::Vector2f(-0.05, 0);

	rect_.setPosition(position);
	rect_.setSize(sf::Vector2f(width, height));
}

void Enemy::update(float time) {

	if (state_ != entities::State::kDead) {
		mapProcessing();
		rect_.move(speed_ * time);
	}
	else if (!is_size_changed_) {
		is_size_changed_ = true;
		rect_.move(0.f, 8.f);
		rect_.scale(1.f, 0.5f);
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

	animation_player_.update(rect_, time);
}

void Enemy::mapProcessing() {

	for (auto object : level_objects_)
		if (rect_.getGlobalBounds().intersects(object->rect_.getGlobalBounds())) {
			if ((object->type_ == SceneObject::Type::kSolid) || (object->type_ == SceneObject::Type::kEnemyBorder)) {
				if (direction_ == entities::MoveDirection::kRight) {
					setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x);
					direction_ = entities::MoveDirection::kLeft;
					speed_.x *= -1;
				} else {
					setPositionX(rect_, object->rect_.getPosition().x + object->rect_.getSize().x);
					direction_ = entities::MoveDirection::kRight;
					speed_.x *= -1;
				}
			}
		}
}