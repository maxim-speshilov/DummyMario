#include "Entity.h"
#include "AnimationPlayer.h"

Entity::Entity(SceneObject::Type type, const TextureHolder &textures) :
	animation_player_(type, textures),
	hp_(0.f),
	speed_(0.f, 0.f) {	
	type_ = type;
}

Entity::Entity() {

}

void Entity::update(float dt, CommandQueue& commands) {
	rect_.move(speed_ * dt);
	animation_player_.update(dt);
}

void Entity::draw(sf::RenderTarget &rt, sf::RenderStates states) const {
	rt.draw(rect_, states);
}

void Entity::updateDirection() {

}

entities::MoveDirection Entity::getDirection() const {
	return direction_;
}

entities::State Entity::getState() const {
	return state_;
}

sf::Vector2f Entity::getSpeed() const {
	return speed_;
}

void Entity::setDirection(entities::MoveDirection dir) {
	direction_ = dir;
}

void Entity::setState(entities::State state) {
	state_ = state;
}

void Entity::setSpeed(const sf::Vector2f& speed){
	speed_ = speed;
}

void Entity::setSpeed(float speed_x, float speed_y)	{
	speed_.x = speed_x;
	speed_.y = speed_y;
}

void Entity::setSpeedX(float speed_x) {
	speed_.x = speed_x;
}

void Entity::setSpeedY(float speed_y) {
	speed_.y = speed_y;
}

void Entity::damage(float hp) {
	hp_ -= hp;
}

void Entity::heal(float hp) {
	hp_ += hp;
}

