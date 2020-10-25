#include "Entity.h"

using namespace entity;

Entity::Entity(Scene& level, Vector2f pos, int width, int height){
	std::vector <Object> level_objects_ = level.getAllObjects();
	for (Object object : level_objects_)
		this->level_objects_.push_back(std::make_shared<Object>(object));	
	rect_ = FloatRect(pos.x, pos.y, width, height);
}

Entity::~Entity() { }

MoveDirection Entity::getDirection() {
	return this->direction_;
}

// TODO(me): Sliding processing while being on slope  

Player::Player(Scene& level, Vector2f pos, int width, int height) :
	Entity(level, pos, width, height) {
	state_ = kStaying;
	type_ = Type::kPlayer;
	is_on_ground_ = true;
	speed_ = Vector2f(0, 0);
}

void Player::mapProcessing(int direction_){

	for (auto object : level_objects_){
		if (rect_.intersects(object->rect_)){
			if (object->type_ == "Solid"){
				if (direction_ == 1){
					if (speed_.y > 0){
						rect_.top = object->rect_.top - rect_.height;
						speed_.y = 0;
						is_on_ground_ = true;
						state_ = kStaying;
					}

					if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0){
					if (speed_.x > 0)
						rect_.left = object->rect_.left - rect_.width;

					if (speed_.x < 0)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "JumpBooster"){
				if (direction_ == 1){
					if (speed_.y > 0){
						speed_.y = -0.8;
					}
					else if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0){
					if (rect_.left < object->rect_.left - rect_.width)
						rect_.left = object->rect_.left - rect_.width;

					if (rect_.left > object->rect_.left + object->rect_.width)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "JumpSuperBooster"){
				if (direction_ == 1){
					if (speed_.y > 0){
						speed_.y = -1.;
					}

					else if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0){
					if (rect_.left < object->rect_.left - rect_.width)
						rect_.left = object->rect_.left - rect_.width;

					if (rect_.left > object->rect_.left + object->rect_.width)
						rect_.left = object->rect_.left + object->rect_.width;
				}
			}

			if (object->type_ == "LeftPipe"){
				if (direction_ == 1){
					if (speed_.y > 0){
						rect_.top = object->rect_.top - rect_.height;
						speed_.y = 0;
						is_on_ground_ = true;
						state_ = kStaying;
					}

					if (speed_.y < 0) {
						rect_.top = object->rect_.top + object->rect_.height;
						speed_.y = 0;
					}
				}

				if (direction_ == 0){
					Time t = milliseconds(100);

					sleep(t);

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
						state_ = kSliding;
					}

					if (speed_.y < 0) {
						;
					}
				}
			}
		}
	}
}

void Player::update(float time){

	if (speed_.y != 0) {
		state_ = kJumping;
	}

	keyboardProcessing();

	switch (state_){
	case kStaying:
		editor_.setAnimation(animation::kStaying);
		break;
	case kRunning:
		editor_.setAnimation(animation::kRunning);
		break;
	case kJumping:
		if (!is_on_ground_)
			editor_.setAnimation(animation::kJumping);
		break;
	case kRolling:
		editor_.setAnimation(animation::kRolling);
		break;
	case kSwimming:
		editor_.setAnimation(animation::kSwimming);
		break;
	}

	if (direction_ == kLeft)
		editor_.setReflex(true);
	if (direction_ == kRight)
		editor_.setReflex(false);

	speed_.y += 0.002*time;

	rect_.left += speed_.x * time;

	mapProcessing(0);

	rect_.top += speed_.y*time;

	mapProcessing(1);

	editor_.shiftAnimation(time);

	is_key_pressed_["Left"] = is_key_pressed_["Right"] = is_key_pressed_["Up"] = is_key_pressed_["Down"] = false;

}

void Player::keyboardProcessing(){
	if (is_key_pressed_["Left"]){
		if (state_ == kStaying)
			state_ = kRunning;
		direction_ = kLeft;
		this->speed_.x = -key_speed_.x;
	}

	if (is_key_pressed_["Right"]){
		if (state_ == kStaying)
			state_ = kRunning;
		direction_ = kRight;
		this->speed_.x = key_speed_.x;
	}

	if (is_key_pressed_["Up"]){
		if (state_ == kSwimming)
			speed_.y = -key_speed_.y / 10;
		else if (state_ != kJumping){
			state_ = kJumping;
			is_on_ground_ = false;
			speed_.y = -key_speed_.y;
		}
	}

	if (is_key_pressed_["Down"]){
		if (state_ == kSwimming)
			speed_.y = key_speed_.y / 10;
		else if (state_ == kRunning){
			state_ = kRolling;
		}
	}

	if (!(is_key_pressed_["Left"] || is_key_pressed_["Right"])){
		if (state_ == kRunning)
			state_ = kStaying;
		speed_.x = 0;
	}

	if (!is_key_pressed_["Down"]){
		if (state_ == kRolling)
			state_ = kRunning;
	}

	if (!(is_key_pressed_["Up"] || is_key_pressed_["Down"])){
		if (state_ == kSwimming)
			speed_.y = 0;
	}


}

void Player::setKeySpeed(Vector2f speed_) {
	key_speed_ = speed_;
}

Enemy::Enemy(Scene& level, Vector2f position, int width, int height, MoveDirection direction_):
	Entity(level, position, width, height){
	state_ = State::kRunning;
	type_ = Type::kEnemy;
	this->direction_ = direction_;

	if (direction_ == MoveDirection::kRight)
		speed_ = Vector2f(0.05, 0);
	else 
		speed_ = Vector2f(-0.05, 0);
}

void Enemy::update(float time){
	
	if (state_ != State::kDead) {
		mapProcessing();
		rect_.left += speed_.x * time;
	}

	else if (!is_size_changed_){
		rect_.top += 8;
		is_size_changed_ = true;
	}

	switch (state_){
	case State::kRunning:
		editor_.setAnimation(animation::kRunning);
		break;
	case State::kDead:
		editor_.setAnimation(animation::kDead);
		break;
	}

	editor_.shiftAnimation(time);
}

void Enemy::mapProcessing(){

	for (auto object : level_objects_)
		if (rect_.intersects(object->rect_)) {
			if ((object->type_ == "Solid") || (object->type_ == "EnemyBorder")) {
				if (direction_ == MoveDirection::kRight) {
					rect_.left = object->rect_.left - rect_.width;
						direction_ = MoveDirection::kLeft;
						speed_.x *= -1;
				}
				else {
					rect_.left = object->rect_.left + object->rect_.width;
						direction_ = MoveDirection::kRight;
						speed_.x *= -1;
				}
			}
		}
}

MovingPlatform::MovingPlatform(Scene& level, Vector2f position, int width, int height, MoveDirection direction_) :
	Entity(level, position, width, height){

	state_ = State::kRunning;
	this->direction_ = direction_;

	if (direction_ == MoveDirection::kRight)
		speed_ = Vector2f(0.1, 0);
	if (direction_ == MoveDirection::kLeft)
		speed_ = Vector2f(-0.1, 0);
	if (direction_ == MoveDirection::kUp)
		speed_ = Vector2f(0, -0.1);
	if (direction_ == MoveDirection::kDown)
		speed_ = Vector2f(0, 0.1);
	
	if ((direction_ == kLeft) || (direction_ == kRight))
		type_ = Type::kMovingPlatform;
	else
		type_ = Type::kMovingVerticallyPlatform;

	time_to_turn_ = 0;	
}

void MovingPlatform::update(float time){
	if ((direction_ == kLeft) || (direction_ == kRight))
		rect_.left += speed_.x * time;
	if ((direction_ == kUp) || (direction_ == kDown))
		rect_.top += speed_.y * time;

	mapProcessing();
	if ((direction_ == kLeft) || (direction_ == kRight))
		editor_.setAnimation(animation::kRunning);
	if ((direction_ == kUp) || (direction_ == kDown))
		editor_.setAnimation(animation::kClimbing);

	editor_.shiftAnimation(time);
}

void MovingPlatform::mapProcessing()
{
	for (auto object : level_objects_)
		if (rect_.intersects(object->rect_)) {
			if (object->type_ == "Solid" || object->type_ == "PlatformBorder") {
				if (direction_ == MoveDirection::kRight) {
					rect_.left = object->rect_.left - rect_.width;
					direction_ = MoveDirection::kLeft;
					speed_.x *= -1;
				}
				else if (direction_ == MoveDirection::kLeft) {
					rect_.left = object->rect_.left + object->rect_.width;
					direction_ = MoveDirection::kRight;
					speed_.x *= -1;
				}
				else if (direction_ == MoveDirection::kUp) {
					rect_.top = object->rect_.top + object->rect_.height;
					direction_ = MoveDirection::kDown;
					speed_.y *= -1;
				}
				else if (direction_ == MoveDirection::kDown) {
					rect_.top = object->rect_.top - rect_.height;
					direction_ = MoveDirection::kUp;
					speed_.y *= -1;
				}
			}
		}
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