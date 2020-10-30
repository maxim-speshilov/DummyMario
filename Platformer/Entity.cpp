#include "Entity.h"

Entity::Entity(Scene& level, Vector2f pos, int width, int height) {
	std::vector <Object> level_objects_ = level.getAllObjects();

	for (Object object : level_objects_)
		this->level_objects_.push_back(std::make_shared<Object>(object));	

	rect_ = FloatRect(pos.x, pos.y, width, height);
}

Entity::Entity() {

}

Entity::~Entity() { 

}

Entity::MoveDirection Entity::getDirection() {
	return this->direction_;
}

Coin::Coin(Scene& level, Vector2f position, int width, int height) :
	Entity(level, position, width, height) {
	speed_ = Vector2f(0, 0);
	direction_ = MoveDirection::kNowhere;
	type_ = Type::kCoin;
}


void Coin::update(float time) {
	editor_.setAnimation(animation::kSpinning);
	editor_.shiftAnimation(time);
}

ExtraLife::ExtraLife(Scene& scene, Vector2f position, int width, int height) :
	Entity(scene, position, width, height) {
	speed_ = Vector2f(0, 0);
	direction_ = MoveDirection::kNowhere;
	type_ = Type::kExtraLife;
}

void ExtraLife::update(float time) {
	editor_.setAnimation(animation::kStaying);
	editor_.shiftAnimation(time);
}