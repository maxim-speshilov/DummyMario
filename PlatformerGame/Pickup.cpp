#include "Pickup.h"
#include <iostream>

Pickup::Pickup(SceneObject::Type type, const TextureHolder& textures) :
	Entity(type, textures),
	hp_(1.f) {
	setSize(32.f, 32.f);
	assert(type == SceneObject::Type::kCoin || type == SceneObject::Type::kExtraLife);
	switch (type) {
	case SceneObject::Type::kCoin:
		rect_.setTexture(&textures.get(Textures::kCoin));
		rect_.setTextureRect(sf::IntRect(0, 0, 32, 32));
		animation_player_.set(Animation::ID::kSpinning);
		break;
	case SceneObject::Type::kExtraLife:
		rect_.setTexture(&textures.get(Textures::kFullHeart));
		rect_.setTextureRect(sf::IntRect(0, 0, 32, 32));
		animation_player_.set(Animation::ID::kStaying);
		break;
	default:
		break;
	}

}

void Pickup::update(float dt, CommandQueue& commands) {
	animation_player_.update(rect_, dt);
	if (isDestroyed())
		setState(entities::State::kDead);
}

void Pickup::damage(float hp) {
	hp_ -= hp_;
}

bool Pickup::isDestroyed() const {
	return hp_ <= 0.f;
}
