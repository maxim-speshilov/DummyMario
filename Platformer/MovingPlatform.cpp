#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(Scene& level, Vector2f position, int width, int height, MoveDirection direction) :
	Entity(level, position, width, height) {

	state_ = State::kRunning;
	this->direction_ = direction;

	switch (direction) {
	case MoveDirection::kRight:
		speed_ = Vector2f(0.1, 0);
		break;
	case MoveDirection::kLeft:
		speed_ = Vector2f(-0.1, 0);
		break;
	case MoveDirection::kUp:
		speed_ = Vector2f(0., -0.1);
		break;
	case MoveDirection::kDown:
		speed_ = Vector2f(0., 0.1);
		break;
	default:
		speed_ = Vector2f(0., 0.);
		break;
	}

	if ((direction == MoveDirection::kLeft) || (direction == MoveDirection::kRight))
		type_ = Type::kMovingPlatform;
	else
		type_ = Type::kMovingVerticallyPlatform;

	time_to_turn_ = 0;
}

void MovingPlatform::update(float time) {
	if ((direction_ == MoveDirection::kLeft) || (direction_ == MoveDirection::kRight))
		rect_.left += speed_.x * time;
	if ((direction_ == MoveDirection::kUp) || (direction_ == MoveDirection::kDown))
		rect_.top += speed_.y * time;

	mapProcessing();

	if ((direction_ == MoveDirection::kLeft) || (direction_ == MoveDirection::kRight))
		editor_.setAnimation(animation::kRunning);
	if ((direction_ == MoveDirection::kUp) || (direction_ == MoveDirection::kDown))
		editor_.setAnimation(animation::kClimbing);

	editor_.shiftAnimation(time);
}

void MovingPlatform::mapProcessing()
{
	for (auto object : level_objects_)
		if (rect_.intersects(object->rect_)) {
			if (object->type_ == "Solid" || object->type_ == "PlatformBorder") {
				switch (direction_) {
				case MoveDirection::kRight:
					rect_.left = object->rect_.left - rect_.width;
					direction_ = MoveDirection::kLeft;
					speed_.x *= -1;
					break;
				case MoveDirection::kLeft:
					rect_.left = object->rect_.left + object->rect_.width;
					direction_ = MoveDirection::kRight;
					speed_.x *= -1;
					break;
				case MoveDirection::kUp:
					rect_.top = object->rect_.top + object->rect_.height;
					direction_ = MoveDirection::kDown;
					speed_.y *= -1;
					break;
				case MoveDirection::kDown:
					rect_.top = object->rect_.top - rect_.height;
					direction_ = MoveDirection::kUp;
					speed_.y *= -1;
					break;
				default:
					break;
				}
			}
		}
}
