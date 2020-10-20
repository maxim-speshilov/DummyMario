#include "Entity.h"

Entity::Entity(Level& level, Vector2f pos, int width, int height){
	levelObjects = level.getAllObjects();
	rect = FloatRect(pos.x, pos.y, width, height);
}

MoveDirection Entity::getDirection() {
	return this->direction;
}

// TODO: Sliding processing while being on slope  

void Player::mapProcessing(int direction){

	for (Object object : levelObjects){
		if (rect.intersects(object.rect)){
			if (object.type == "Solid"){
				if (direction == 1){
					if (speed.y > 0){
						rect.top = object.rect.top - rect.height;
						speed.y = 0;
						isOnGround = true;
						state = Staying;
					}

					if (speed.y < 0) {
						rect.top = object.rect.top + object.rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (speed.x > 0)
						rect.left = object.rect.left - rect.width;

					if (speed.x < 0)
						rect.left = object.rect.left + object.rect.width;
				}
			}

			if (object.type == "JumpBooster"){
				if (direction == 1){
					if (speed.y > 0){
						speed.y = -0.8;
					}
					else if (speed.y < 0) {
						rect.top = object.rect.top + object.rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (rect.left < object.rect.left - rect.width)
						rect.left = object.rect.left - rect.width;

					if (rect.left > object.rect.left + object.rect.width)
						rect.left = object.rect.left + object.rect.width;
				}
			}

			if (object.type == "JumpSuperBooster"){
				if (direction == 1){
					if (speed.y > 0){
						speed.y = -1.;
					}

					else if (speed.y < 0) {
						rect.top = object.rect.top + object.rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					if (rect.left < object.rect.left - rect.width)
						rect.left = object.rect.left - rect.width;

					if (rect.left > object.rect.left + object.rect.width)
						rect.left = object.rect.left + object.rect.width;
				}
			}

			if (object.type == "LeftPipe"){
				if (direction == 1){
					if (speed.y > 0){
						rect.top = object.rect.top - rect.height;
						speed.y = 0;
						isOnGround = true;
						state = Staying;
					}

					if (speed.y < 0) {
						rect.top = object.rect.top + object.rect.height;
						speed.y = 0;
					}
				}

				if (direction == 0){
					Time t = milliseconds(100);

					sleep(t);

					if (speed.x > 0)
						rect.left = object.rect.left + object.rect.width;

					if (speed.x < 0)
						rect.left = object.rect.left - object.rect.width;
				}
			}

			if (object.type == "SlopeRight") {
				if (direction == 1) {
					if (speed.y > 0) {
						rect.top = object.rect.top - rect.height;
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

Player::Player(Level& level, Vector2f pos ,int width, int height) : 
Entity(level, pos, width, height){
	state = Staying;
	type = "player";
	isOnGround = true;
	speed = Vector2f(0, 0);
}

void Player::update(float time){

	if (speed.y != 0) {
		state = Jumping;
	}

	keyboardProcessing();

	switch (state){
	case Staying:
		editor.setAnimation("Staying");
		break;
	case Running:
		editor.setAnimation("Running");
		break;
	case Jumping:
		if (!isOnGround)
			editor.setAnimation("Jumping");
		break;
	case Rolling:
		editor.setAnimation("Rolling");
		break;
	case Swimming:
		editor.setAnimation("Swimming");
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
		speed.x = -0.1;
	}

	if (isKeyPressed["Right"]){
		if (state == Staying)
			state = Running;
		direction = Right;
		speed.x = 0.1;
	}

	if (isKeyPressed["Up"]){
		if (state == Swimming)
			speed.y = -0.05;
		else if (state != Jumping){
			state = Jumping;
			isOnGround = false;
			speed.y = -0.65;
		}
	}

	if (isKeyPressed["Down"]){
		if (state == Swimming)
			speed.y = 0.05;
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

Enemy::Enemy(Level& level, Vector2f position, int width, int height, MoveDirection direction):
	Entity(level, position, width, height){
	state = Running;
	type = "enemy";
	this->direction = direction;

	if (direction == Right)
		speed = Vector2f(0.05, 0);
	else 
		speed = Vector2f(-0.05, 0);
}

void Enemy::update(float time){
	
	if (state != Dead) {
		mapProcessing();
		rect.left += speed.x * time;
	}

	else if (!isSizeChanged){
		rect.top += 8;
		isSizeChanged = true;
	}

	switch (state){
	case Running:
		editor.setAnimation("Running");
		break;
	case Dead:
		editor.setAnimation("Dead");
		break;
	}

	editor.shiftAnimation(time);
}

void Enemy::mapProcessing(){

	for (Object object : levelObjects)
		if (rect.intersects(object.rect)) {
			if (object.type == "Solid") {
				if (direction == Right) {
					rect.left = object.rect.left - rect.width;
						direction = Left;
						speed.x *= -1;
				}
				else {
					rect.left = object.rect.left + object.rect.width;
						direction = Right;
						speed.x *= -1;
				}
			}
		}
}

MovingPlatform::MovingPlatform(Level& level, Vector2f position, int width, int height) :
	Entity(level, position, width, height){
	speed = Vector2f(0.1, 0);
	direction = Right;
	type = "moving platform";
	timeToTurn = 0;	
}

void MovingPlatform::update(float time){
	rect.left += speed.x * time;
	timeToTurn += time;
	if (timeToTurn > 3000){
		speed.x *= -1;
		timeToTurn = 0;
	}

	editor.setAnimation("Moving");
	editor.shiftAnimation(time);
}

void MovingPlatform::mapProcessing()
{
}


Coin::Coin(Level& level, Vector2f position, int width, int height) :
	Entity(level, position, width, height) {
	speed = Vector2f(0, 0);
	direction = Nowhere;
	type = "coin";
}


void Coin::update(float time) {
	editor.setAnimation("Spinning");
	editor.shiftAnimation(time);
}
