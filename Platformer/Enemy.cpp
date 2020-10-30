#include "Enemy.h"

Enemy::Enemy(Scene& level, Vector2f position, int width, int height, Entity::MoveDirection direction_) :
	Entity(level, position, width, height) {
	state_ = Entity::State::kRunning;
	type_ = Entity::Type::kEnemy;
	this->direction_ = direction_;

	if (direction_ == Entity::MoveDirection::kRight)
		speed_ = Vector2f(0.05, 0);
	else
		speed_ = Vector2f(-0.05, 0);
}

void Enemy::update(float time) {

	if (state_ != Entity::State::kDead) {
		mapProcessing();
		rect_.left += speed_.x * time;
	}
	else if (!is_size_changed_) {
		rect_.top += 8;
		is_size_changed_ = true;
	}

	switch (state_) {
	case Entity::State::kRunning:
		editor_.setAnimation(animation::kRunning);
		break;
	case Entity::State::kDead:
		editor_.setAnimation(animation::kDead);
		break;
	default:
		break;
	}

	editor_.shiftAnimation(time);
}

void Enemy::mapProcessing() {

	for (auto object : level_objects_)
		if (rect_.intersects(object->rect_)) {
			if ((object->type_ == "Solid") || (object->type_ == "EnemyBorder")) {
				if (direction_ == Entity::MoveDirection::kRight) {
					rect_.left = object->rect_.left - rect_.width;
					direction_ = Entity::MoveDirection::kLeft;
					speed_.x *= -1;
				}
				else {
					rect_.left = object->rect_.left + object->rect_.width;
					direction_ = Entity::MoveDirection::kRight;
					speed_.x *= -1;
				}
			}
		}
}