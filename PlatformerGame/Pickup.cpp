#include "Pickup.h"

Pickup::Pickup(entities::Type type, Scene& level, sf::Vector2f position, int width, int height) :
	Entity(level, position, width, height) {
	type_ = type;
}

Pickup::Pickup(Scene& level, entities::Type type, const TextureHolder& textures, sf::Vector2f position, int width, int height) :
	Entity(level, type, textures, position, width, height) {
	rect_.setPosition(position);
	rect_.setSize(sf::Vector2f(width, height));
}

void Pickup::update(float dt) {
	switch (type_) {
	case entities::Type::kCoin:
		animation_player_.set(Animation::ID::kSpinning);
		break;
	case entities::Type::kExtraLife:
		animation_player_.set(Animation::ID::kStaying);
		break;
	default:
		throw std::logic_error("Pickup::update - got a pickup of an undefined type");
	}

	animation_player_.update(rect_, dt);
}
