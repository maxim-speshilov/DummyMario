#include "Boomerang.h"

Boomerang::Boomerang(const TextureHolder& textures) : 
	Entity(SceneObject::Type::kBoomerang, textures) {
	hp_ = 1.f;
	state_ = entities::State::kRunning;
	animation_player_.set(Animation::ID::kRunning);
	setSpeed(sf::Vector2f(164.f, 0.f));
	direction_ = entities::MoveDirection::kRight;
	rect_.setSize(sf::Vector2f(16, 16));
	rect_.setTexture(&textures.get(Textures::kBoomerang));
	rect_.setTextureRect(sf::IntRect(0, 0, 16, 16));
}

void Boomerang::update(float dt, CommandQueue& commands) {
	auto ds = speed_ * dt;
	travelled_distance_ += ds.x;
	rect_.move(ds);
	animation_player_.update(rect_, dt);
}

bool Boomerang::isDestroyed() const {
	return hp_ <= 0.f;
}

bool Boomerang::isMarkedForRemoval() const {
	return isDestroyed() || (travelled_distance_ >= 200.f);
}
