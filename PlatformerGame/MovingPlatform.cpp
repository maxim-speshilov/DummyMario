#include "MovingPlatform.h"
#include <iostream>


MovingPlatform::MovingPlatform(SceneObject::Type type, const TextureHolder& textures) :
	Entity(type, textures) {

	switch (type) {
	case SceneObject::Type::kMovingPlatform:
		animation_player_.set(Animation::ID::kRunning);
		setDirection(entities::MoveDirection::kRight);
		rect_.setTexture(&textures.get(Textures::kMovingPlatform));
		rect_.setTextureRect(sf::IntRect(0, 0, 96, 32));
		setSpeed(82.f, 0.f);
		setSize(96.f, 32.f);
		break;
	case SceneObject::Type::kMovingVerticallyPlatform:
		animation_player_.set(Animation::ID::kClimbing);
		direction_ = entities::MoveDirection::kUp;
		rect_.setTexture(&textures.get(Textures::kMovingVerticallyPlatform));
		rect_.setTextureRect(sf::IntRect(0, 0, 32, 32));
		setSpeed(0.f, -82.f);
		setSize(32.f, 32.f);
		break;
	default:
		throw std::logic_error("MovingPlatform::MovingPlatform - got wrong type");
		break;
	}
}


