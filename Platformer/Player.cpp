#include "Player.h"

// TODO(me) : Sliding processing while being on slope

Player::Player(Scene& level, Vector2f pos, int width, int height) :
	Entity(level, pos, width, height) {
	state_ = State::kStaying;
	type_ = Type::kPlayer;
	is_on_ground_ = true;
	speed_ = Vector2f(0, 0);
}

Player::Player() :
	Entity() {
	state_ = State::kStaying;
	type_ = Type::kPlayer;
	is_on_ground_ = true;
	speed_ = Vector2f(0, 0);
}

void Player::mapProcessing(int direction_) {

	for (auto object : level_objects_) {
		if (rect_.intersects(object->rect_)) {
			if (object->type_ == "Solid") {
				if (direction_ == 1) {
					if (speed_.y > 0) {
						rect_.top = object->rect_.top - rect_.height;
						speed_.y = 0;
						is_on_ground_ = true;
						state_ = State::kStaying;
					}

					if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0) {
					if (speed_.x > 0)
						rect_.left = object->rect_.left - rect_.width;

					if (speed_.x < 0)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "JumpBooster") {
				if (direction_ == 1) {
					if (speed_.y > 0) {
						speed_.y = -0.8;
					}
					else if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0) {
					if (rect_.left < object->rect_.left - rect_.width)
						rect_.left = object->rect_.left - rect_.width;

					if (rect_.left > object->rect_.left + object->rect_.width)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "JumpSuperBooster") {
				if (direction_ == 1) {
					if (speed_.y > 0) {
						speed_.y = -1.;
					}
					else if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0) {
					if (rect_.left < object->rect_.left - rect_.width)
						rect_.left = object->rect_.left - rect_.width;

					if (rect_.left > object->rect_.left + object->rect_.width)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "LeftPipe") {
				if (direction_ == 1) {
					if (speed_.y > 0) {
						rect_.top = object->rect_.top - rect_.height;
						speed_.y = 0;
						is_on_ground_ = true;
						state_ = State::kStaying;
					}

					if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0) {
					if (speed_.x > 0)
						rect_.left = object->rect_.left + object->rect_.width;

					if (speed_.x < 0)
						rect_.left = object->rect_.left - object->rect_.width;
				}
			}

			if (object->type_ == "SlopeRight") {
				if (direction_ == 1) {
					if (speed_.y > 0) {
						rect_.top = object->rect_.top - rect_.height;
						speed_.y *= 0.4472;
						is_on_ground_ = true;
						state_ = State::kSliding;
					}

					if (speed_.y < 0) {
						;
					}
				}
			}
		}
	}
}

void Player::update(float time) {

	if (speed_.y != 0)
		state_ = State::kJumping;

	keyboardProcessing();

	switch (state_) {
	case State::kStaying:
		editor_.setAnimation(animation::kStaying);
		break;
	case State::kRunning:
		editor_.setAnimation(animation::kRunning);
		break;
	case State::kJumping:
		if (!is_on_ground_)
			editor_.setAnimation(animation::kJumping);
		break;
	case State::kRolling:
		editor_.setAnimation(animation::kRolling);
		break;
	case State::kSwimming:
		editor_.setAnimation(animation::kSwimming);
		break;
	default:
		break;
	}

	if (direction_ == MoveDirection::kLeft)
		editor_.setReflex(true);
	if (direction_ == MoveDirection::kRight)
		editor_.setReflex(false);

	speed_.y += 0.002*time;
	rect_.left += speed_.x * time;
	mapProcessing(0);
	rect_.top += speed_.y*time;
	mapProcessing(1);
	editor_.shiftAnimation(time);
	is_key_pressed_["Left"] = is_key_pressed_["Right"] = is_key_pressed_["Up"] = is_key_pressed_["Down"] = false;
}

void Player::keyboardProcessing() {
	if (is_key_pressed_["Left"]) {
		if (state_ == State::kStaying)
			state_ = State::kRunning;
		direction_ = MoveDirection::kLeft;
		this->speed_.x = -key_speed_.x;
	}

	if (is_key_pressed_["Right"]) {
		if (state_ == State::kStaying)
			state_ = State::kRunning;
		direction_ = MoveDirection::kRight;
		this->speed_.x = key_speed_.x;
	}

	if (is_key_pressed_["Up"]) {
		if (state_ == State::kSwimming)
			speed_.y = -key_speed_.y / 10;
		else if (state_ != State::kJumping) {
			state_ = State::kJumping;
			is_on_ground_ = false;
			speed_.y = -key_speed_.y;
		}
	}

	if (is_key_pressed_["Down"]) {
		if (state_ == State::kSwimming)
			speed_.y = key_speed_.y / 10;
		else if (state_ == State::kRunning) {
			state_ = State::kRolling;
		}
	}

	if (!(is_key_pressed_["Left"] || is_key_pressed_["Right"])) {
		if (state_ == State::kRunning)
			state_ = State::kStaying;
		speed_.x = 0;
	}

	if (!is_key_pressed_["Down"]) {
		if (state_ == State::kRolling)
			state_ = State::kRunning;
	}

	if (!(is_key_pressed_["Up"] || is_key_pressed_["Down"])) {
		if (state_ == State::kSwimming)
			speed_.y = 0;
	}


}

void Player::setKeySpeed(Vector2f speed_) {
	key_speed_ = speed_;
}
