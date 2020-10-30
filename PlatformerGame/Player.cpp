#include "Player.h"

// TODO(me) : Sliding processing while being on slope

Animation::ID toAnimationID(entities::State state) {
	return Animation::ID();
}

Player::Player(Scene& level, sf::Vector2f pos, int width, int height) :
	Entity(level, pos, width, height) {
	state_ = entities::State::kStaying;
	type_ = entities::Type::kPlayer;
	is_on_ground_ = true;
	speed_ = sf::Vector2f(0, 0);
	rect_.setPosition(pos);
	rect_.setSize(sf::Vector2f(width, height));
}

Player::Player(Scene& level, const TextureHolder& textures, sf::Vector2f pos, int width, int height) :
	Entity(level, entities::Type::kPlayer, textures, pos, width, height) {
	state_ = entities::State::kStaying;
	type_ = entities::Type::kPlayer;
	is_on_ground_ = true;
	speed_ = sf::Vector2f(0, 0);
	rect_.setPosition(pos);
	rect_.setSize(sf::Vector2f(width, height));
}

Player::Player() :
	Entity() {
	state_ = entities::State::kStaying;
	type_ = entities::Type::kPlayer;
	is_on_ground_ = true;
	speed_ = sf::Vector2f(0, 0);
}

void Player::mapProcessing(int direction_) {

	for (auto object : level_objects_) {
		if (rect_.getGlobalBounds().intersects(object->rect_.getGlobalBounds())) {
			switch (object->type_) {
			case SceneObject::Type::kSolid:
				if (direction_ == 1) {
					if (speed_.y > 0.f) {
						setPositionY(rect_, object->rect_.getPosition().y - rect_.getSize().y);
						speed_.y = 0.f;
						is_on_ground_ = true;
						state_ = entities::State::kStaying;
					}

					if (speed_.y < 0.f) {
						setPositionY(rect_, object->rect_.getPosition().y + object->rect_.getSize().y);
						speed_.y = 0.f;
					}
				}

				if (direction_ == 0) {
					if (speed_.x > 0.f)
						setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x);

					if (speed_.x < 0.f)
						setPositionX(rect_, object->rect_.getPosition().x + object->rect_.getSize().x);
				}

				break;

			case SceneObject::Type::kJumpBooster:
					if (direction_ == 1) {
						if (speed_.y > 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y - rect_.getSize().y);
							speed_.y = -0.8f;
						} else if (speed_.y < 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y + object->rect_.getSize().y);
							speed_.y = 0.f;
						}
					}

					if (direction_ == 0) {
						if (speed_.x > 0)
							setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x);

						if (speed_.x < 0)
							setPositionX(rect_, object->rect_.getPosition().x + object->rect_.getSize().x);
					}
				
					break;

			case SceneObject::Type::kJumpSuperBooster:
					if (direction_ == 1) {
						if (speed_.y > 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y - rect_.getSize().y);
							speed_.y = -1.f;
						}
						else if (speed_.y < 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y + object->rect_.getSize().y);
							speed_.y = 0.f;
						}
					}

					if (direction_ == 0) {
						if (speed_.x > 0)
							setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x);

						if (speed_.x < 0)
							setPositionX(rect_, object->rect_.getPosition().x + object->rect_.getSize().x);
						
					}
					break;
				
			case SceneObject::Type::kLeftPipe:
					if (direction_ == 1) {
						if (speed_.y > 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y - rect_.getSize().y);
							speed_.y = 0.f;
							is_on_ground_ = true;
							state_ = entities::State::kStaying;
						}

						if (speed_.y < 0.f) {
							setPositionY(rect_, object->rect_.getPosition().y + object->rect_.getSize().y);
							speed_.y = 0.f;
						}
					}

					if (direction_ == 0) {
						if (speed_.x > 0.f)
							setPositionX(rect_, object->rect_.getPosition().x + object->rect_.getSize().x);

						if (speed_.x < 0.f)
							setPositionX(rect_, object->rect_.getPosition().x - rect_.getSize().x - 2);
					}

					break;

			case SceneObject::Type::kSlopeRight:
				break;

			default:
				break;
			}
		}
	}
}

void Player::update(float time) {

	if (speed_.y != 0)
		state_ = entities::State::kJumping;

	keyboardProcessing();

	switch (state_) {
	case entities::State::kStaying:
		animation_player_.set(Animation::ID::kStaying);
		break;
	case entities::State::kRunning:
		animation_player_.set(Animation::ID::kRunning);
		break;
	case entities::State::kJumping:
		if (!is_on_ground_)
			animation_player_.set(Animation::ID::kJumping);
		break;
	case entities::State::kRolling:
		animation_player_.set(Animation::ID::kRolling);
		break;
	case entities::State::kSwimming:
		animation_player_.set(Animation::ID::kSwimming);
		break;
	default:
		break;
	}

	if (direction_ == entities::MoveDirection::kLeft)
		animation_player_.setReflex(true);
	if (direction_ == entities::MoveDirection::kRight)
		animation_player_.setReflex(false);

	speed_.y += 0.002*time;
	rect_.move(sf::Vector2f(speed_.x * time, 0.f));
	mapProcessing(0);
	rect_.move(sf::Vector2f(0.f, speed_.y * time));
	mapProcessing(1);

	animation_player_.update(rect_, time);
	is_key_pressed_["Left"] = is_key_pressed_["Right"] = is_key_pressed_["Up"] = is_key_pressed_["Down"] = false;
}


void Player::keyboardProcessing() {
	if (is_key_pressed_["Left"]) {
		if (state_ == entities::State::kStaying)
			state_ = entities::State::kRunning;
		direction_ = entities::MoveDirection::kLeft;
		speed_.x = -key_speed_.x;
	}

	if (is_key_pressed_["Right"]) {
		if (state_ == entities::State::kStaying)
			state_ = entities::State::kRunning;
		direction_ = entities::MoveDirection::kRight;
		speed_.x = key_speed_.x;
	}

	if (is_key_pressed_["Up"]) {
		if (state_ == entities::State::kSwimming)
			speed_.y = -key_speed_.y / 10.f;
		else if (state_ != entities::State::kJumping) {
			state_ = entities::State::kJumping;
			is_on_ground_ = false;
			speed_.y = -key_speed_.y;
		}
	}

	if (is_key_pressed_["Down"]) {
		if (state_ == entities::State::kSwimming)
			speed_.y = key_speed_.y / 10.f;
		else if (state_ == entities::State::kRunning) {
			state_ = entities::State::kRolling;
		}
	}

	if (!(is_key_pressed_["Left"] || is_key_pressed_["Right"])) {
		if (state_ == entities::State::kRunning)
			state_ = entities::State::kStaying;
		speed_.x = 0.f;
	}

	if (!is_key_pressed_["Down"]) {
		if (state_ == entities::State::kRolling)
			state_ = entities::State::kRunning;
	}

	if (!(is_key_pressed_["Up"] || is_key_pressed_["Down"])) {
		if (state_ == entities::State::kSwimming)
			speed_.y = 0.f;
	}


}

void Player::setKeySpeed(sf::Vector2f speed_) {
	key_speed_ = speed_;
}
