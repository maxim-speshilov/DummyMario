#include "MovingPlatform.h"
#include <iostream>

MovingPlatform::MovingPlatform(Scene& level, sf::Vector2f position, int width, int height, entities::MoveDirection direction) :
	Entity(level, position, width, height) {

	state_ = entities::State::kRunning;
	this->direction_ = direction;

	switch (direction) {
	case entities::MoveDirection::kRight:
		speed_ = sf::Vector2f(0.1, 0);
		break;
	case entities::MoveDirection::kLeft:
		speed_ = sf::Vector2f(-0.1, 0);
		break;
	case entities::MoveDirection::kUp:
		speed_ = sf::Vector2f(0., -0.1);
		break;
	case entities::MoveDirection::kDown:
		speed_ = sf::Vector2f(0., 0.1);
		break;
	default:
		speed_ = sf::Vector2f(0., 0.);
		break;
	}

	if ((direction == entities::MoveDirection::kLeft) || (direction == entities::MoveDirection::kRight))
		type_ = entities::Type::kMovingPlatform;
	else
		type_ = entities::Type::kMovingVerticallyPlatform;

	time_to_turn_ = 0;
}

MovingPlatform::MovingPlatform(Scene& level, const TextureHolder& textures, sf::Vector2f position, int width, int height, entities::MoveDirection direction) :
	Entity(level, position, width, height) {

	state_ = entities::State::kRunning;
	this->direction_ = direction;

	switch (direction) {
	case entities::MoveDirection::kRight:
		speed_ = sf::Vector2f(0.1, 0);
		break;
	case entities::MoveDirection::kLeft:
		speed_ = sf::Vector2f(-0.1, 0);
		break;
	case entities::MoveDirection::kUp:
		speed_ = sf::Vector2f(0., -0.1);
		break;
	case entities::MoveDirection::kDown:
		speed_ = sf::Vector2f(0., 0.1);
		break;
	default:
		speed_ = sf::Vector2f(0., 0.);
		break;
	}

	if ((direction == entities::MoveDirection::kLeft) || (direction == entities::MoveDirection::kRight)) {
		type_ = entities::Type::kMovingPlatform;
		animation_player_.set(Animation::ID::kRunning);
	} else {
		type_ = entities::Type::kMovingVerticallyPlatform;
		animation_player_.set(Animation::ID::kClimbing);
	}

	animation_player_ = AnimationPlayer(type_, textures);
	rect_.setPosition(position);
	rect_.setSize(sf::Vector2f(width, height));
	time_to_turn_ = 0;
}

void MovingPlatform::update(float time) {
	animation_player_.update(time);
	rect_.setTexture(animation_player_.getTexture(), true);
	rect_.move(speed_ * time);
	mapProcessing();
}

void MovingPlatform::draw(sf::RenderTarget & rt, sf::RenderStates states) const {
	rt.draw(rect_, states);
}

void MovingPlatform::mapProcessing()
{
	for (auto object : level_objects_)
		if (rect_.getGlobalBounds().intersects(object->rect_.getGlobalBounds())) {
			if (object->type_ == SceneObject::Type::kSolid || object->type_ == SceneObject::Type::kPlatformBorder) {
				switch (direction_) {
				case entities::MoveDirection::kRight:
					setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x);
					direction_ = entities::MoveDirection::kLeft;
					speed_.x *= -1;
					break;
				case entities::MoveDirection::kLeft:
					setPositionX(rect_, object->rect_.getPosition().x + rect_.getSize().x);
					direction_ = entities::MoveDirection::kRight;
					speed_.x *= -1;
					break;
				case entities::MoveDirection::kUp:
					setPositionY(rect_, object->rect_.getPosition().y + object->rect_.getSize().y);
					direction_ = entities::MoveDirection::kDown;
					speed_.y *= -1;
					break;
				case entities::MoveDirection::kDown:
					setPositionY(rect_, object->rect_.getPosition().y - rect_.getSize().y);
					direction_ = entities::MoveDirection::kUp;
					speed_.y *= -1;
					break;
				default:
					break;
				}
			}
		}
}
