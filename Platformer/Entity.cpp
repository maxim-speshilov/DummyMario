#include "Entity.h"

Entity::Entity(Scene& level, Vector2f pos, int width, int height){
	std::vector <Object> levelObjects = level.getAllObjects();
	for (Object object : levelObjects)
		this->levelObjects.push_back(std::make_shared<Object>(object));	
	rect = FloatRect(pos.x, pos.y, width, height);
}

Entity::~Entity() { }

Entity::MoveDirection Entity::getDirection() {
	return this->direction;
}

// TODO(me): Sliding processing while being on slope  

Player::Player(Scene& level, Vector2f pos, int width, int height) :
	Entity(level, pos, width, height) {
	state = Staying;
	type = EntityType::Player;
	isOnGround = true;
	speed = Vector2f(0, 0);
}

void Player::mapProcessing(int direction){

	for (auto object : levelObjects){
		if (rect.intersects(object->rect)){
			if (object->type == "Solid"){
				if (direction == 1){
					if (speed.y > 0){
						rect.top = object->rect.top - rect.height;
						speed.y = 0;
						isOnGround = true;
						state = Staying;
					}

					if (speed.y < 0) {
						rect.top = object->rect.top + object->rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (speed.x > 0)
						rect.left = object->rect.left - rect.width;

					if (speed.x < 0)
						rect.left = object->rect.left + object->rect.width;
				}
			}

			if (object->type == "JumpBooster"){
				if (direction == 1){
					if (speed.y > 0){
						speed.y = -0.8;
					}
					else if (speed.y < 0) {
						rect.top = object->rect.top + object->rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (rect.left < object->rect.left - rect.width)
						rect.left = object->rect.left - rect.width;

					if (rect.left > object->rect.left + object->rect.width)
						rect.left = object->rect.left + object->rect.width;
				}
			}

			if (object->type == "JumpSuperBooster"){
				if (direction == 1){
					if (speed.y > 0){
						speed.y = -1.;
					}

					else if (speed.y < 0) {
						rect.top = object->rect.top + object->rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (rect.left < object->rect.left - rect.width)
						rect.left = object->rect.left - rect.width;

					if (rect.left > object->rect.left + object->rect.width)
						rect.left = object->rect.left + object->rect.width;
				}
			}

			if (object->type == "LeftPipe"){
				if (direction == 1){
					if (speed.y > 0){
						rect.top = object->rect.top - rect.height;
						speed.y = 0;
						isOnGround = true;
						state = Staying;
					}

					if (speed.y < 0) {
						rect.top = object->rect.top + object->rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					Time t = milliseconds(100);

					sleep(t);

					if (speed.x > 0)
						rect.left = object->rect.left + object->rect.width;

					if (speed.x < 0)
						rect.left = object->rect.left - object->rect.width;
				}
			}

			if (object->type == "SlopeRight") {
				if (direction == 1) {
					if (speed.y > 0) {
						rect.top = object->rect.top - rect.height;
						speed.y *= 0.4472;
						isOnGround = true;
						state = Sliding;
					}

					if (speed.y < 0) {
						;
					}
				}
			}
		}
	}
}

void Player::update(float time){

	if (speed.y != 0) {
		state = Jumping;
	}

	keyboardProcessing();

	switch (state){
	case Staying:
		editor.setAnimation(Animation::AnimationType::Staying);
		break;
	case Running:
		editor.setAnimation(Animation::AnimationType::Running);
		break;
	case Jumping:
		if (!isOnGround)
			editor.setAnimation(Animation::AnimationType::Jumping);
		break;
	case Rolling:
		editor.setAnimation(Animation::AnimationType::Rolling);
		break;
	case Swimming:
		editor.setAnimation(Animation::AnimationType::Swimming);
		break;
	}

	if (direction == Left)
		editor.set_isReflex(true);
	if (direction == Right)
		editor.set_isReflex(false);

	speed.y += 0.002*time;

	rect.left += speed.x * time;

	mapProcessing(0);

	rect.top += speed.y*time;

	mapProcessing(1);

	editor.shiftAnimation(time);

	isKeyPressed["Left"] = isKeyPressed["Right"] = isKeyPressed["Up"] = isKeyPressed["Down"] = false;

}

void Player::keyboardProcessing(){
	if (isKeyPressed["Left"]){
		if (state == Staying)
			state = Running;
		direction = Left;
		this->speed.x = -key_speed.x;
	}

	if (isKeyPressed["Right"]){
		if (state == Staying)
			state = Running;
		direction = Right;
		this->speed.x = key_speed.x;
	}

	if (isKeyPressed["Up"]){
		if (state == Swimming)
			speed.y = -key_speed.y / 10;
		else if (state != Jumping){
			state = Jumping;
			isOnGround = false;
			speed.y = -key_speed.y;
		}
	}

	if (isKeyPressed["Down"]){
		if (state == Swimming)
			speed.y = key_speed.y / 10;
		else if (state == Running){
			state = Rolling;
		}
	}

	if (!(isKeyPressed["Left"] || isKeyPressed["Right"])){
		if (state == Running)
			state = Staying;
		speed.x = 0;
	}

	if (!isKeyPressed["Down"]){
		if (state == Rolling)
			state = Running;
	}

	if (!(isKeyPressed["Up"] || isKeyPressed["Down"])){
		if (state == Swimming)
			speed.y = 0;
	}


}

void Player::setKeySpeed(Vector2f speed) {
	key_speed = speed;
}

Enemy::Enemy(Scene& level, Vector2f position, int width, int height, MoveDirection direction):
	Entity(level, position, width, height){
	state = EntityState::Running;
	type = EntityType::Enemy;
	this->direction = direction;

	if (direction == MoveDirection::Right)
		speed = Vector2f(0.05, 0);
	else 
		speed = Vector2f(-0.05, 0);
}

void Enemy::update(float time){
	
	if (state != EntityState::Dead) {
		mapProcessing();
		rect.left += speed.x * time;
	}

	else if (!isSizeChanged){
		rect.top += 8;
		isSizeChanged = true;
	}

	switch (state){
	case EntityState::Running:
		editor.setAnimation(Animation::AnimationType::Running);
		break;
	case EntityState::Dead:
		editor.setAnimation(Animation::AnimationType::Dead);
		break;
	}

	editor.shiftAnimation(time);
}

void Enemy::mapProcessing(){

	for (auto object : levelObjects)
		if (rect.intersects(object->rect)) {
			if ((object->type == "Solid") || (object->type == "Enemy border")) {
				if (direction == MoveDirection::Right) {
					rect.left = object->rect.left - rect.width;
						direction = MoveDirection::Left;
						speed.x *= -1;
				}
				else {
					rect.left = object->rect.left + object->rect.width;
						direction = MoveDirection::Right;
						speed.x *= -1;
				}
			}
		}
}

MovingPlatform::MovingPlatform(Scene& level, Vector2f position, int width, int height, MoveDirection direction) :
	Entity(level, position, width, height){

	state = EntityState::Running;
	this->direction = direction;

	if (direction == MoveDirection::Right)
		speed = Vector2f(0.1, 0);
	if (direction == MoveDirection::Left)
		speed = Vector2f(-0.1, 0);
	if (direction == MoveDirection::Up)
		speed = Vector2f(0, -0.1);
	if (direction == MoveDirection::Down)
		speed = Vector2f(0, 0.1);
	
	if ((direction == Left) || (direction == Right))
		type = EntityType::MovingPlatform;
	else
		type = EntityType::MovingVerticallyPlatform;

	timeToTurn = 0;	
}

void MovingPlatform::update(float time){
	if ((direction == Left) || (direction == Right))
		rect.left += speed.x * time;
	if ((direction == Up) || (direction == Down))
		rect.top += speed.y * time;

	mapProcessing();
	if ((direction == Left) || (direction == Right))
		editor.setAnimation(Animation::AnimationType::Running);
	if ((direction == Up) || (direction == Down))
		editor.setAnimation(Animation::AnimationType::Climbing);

	editor.shiftAnimation(time);
}

void MovingPlatform::mapProcessing()
{
	for (auto object : levelObjects)
		if (rect.intersects(object->rect)) {
			if (object->type == "Solid" || object->type == "Platform border") {
				if (direction == MoveDirection::Right) {
					rect.left = object->rect.left - rect.width;
					direction = MoveDirection::Left;
					speed.x *= -1;
				}
				else if (direction == MoveDirection::Left) {
					rect.left = object->rect.left + object->rect.width;
					direction = MoveDirection::Right;
					speed.x *= -1;
				}
				else if (direction == MoveDirection::Up) {
					rect.top = object->rect.top + object->rect.height;
					direction = MoveDirection::Down;
					speed.y *= -1;
				}
				else if (direction == MoveDirection::Down) {
					rect.top = object->rect.top - rect.height;
					direction = MoveDirection::Up;
					speed.y *= -1;
				}
			}
		}
}


Coin::Coin(Scene& level, Vector2f position, int width, int height) :
	Entity(level, position, width, height) {
	speed = Vector2f(0, 0);
	direction = MoveDirection::Nowhere;
	type = EntityType::Coin;
}


void Coin::update(float time) {
	editor.setAnimation(Animation::AnimationType::Spinning);
	editor.shiftAnimation(time);
}

ExtraLife::ExtraLife(Scene& scene, Vector2f position, int width, int height) :
	Entity(scene, position, width, height) {
	speed = Vector2f(0, 0);
	direction = MoveDirection::Nowhere;
	type = EntityType::ExtraLife;
}

void ExtraLife::update(float time) {
	editor.setAnimation(Animation::AnimationType::Staying);
	editor.shiftAnimation(time);
}