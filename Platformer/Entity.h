#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <iterator>
#include <SFML\Graphics.hpp>
#include <string>
#include <SFML\Audio.hpp>
#include "Animation.h"A
#include "Scene.h"

using namespace sf;

enum MoveDirection { Nowhere, Right, Left, Up, Down };
enum EntityState { Staying, Running, Jumping, Rolling, Swimming, Climbing, Sliding, Dead, Invulnerable};

// TODO(me): Change update. Change entity to avoid storing vector of pointers to level objects.
class Entity{
protected:
	Vector2f speed;
	float currentFrame;
	FloatRect rect;
	bool isOnGround, isSizeChanged;
	MoveDirection direction;
	EntityState state;
	AnimationEditor editor;
	vector <std::shared_ptr<Object>> levelObjects;
	string type;
public:
	Entity(Scene&, Vector2f, int, int);
	~Entity();
	MoveDirection getDirection();
	virtual void update(float) = 0;
	friend bool gameLoop();
};

// TODO(me): Fix position bug that happens when resizing window.
class Player : public Entity {
private:
	Vector2f key_speed = Vector2f(0.1f, 0.65f);
public:
	Player(Scene&, Vector2f, int, int);
	bool isWounded, isOnPlatform;
	std::map <String, bool> isKeyPressed;
	void update(float) override;
	void mapProcessing(int);
	void keyboardProcessing();
	void setKeySpeed(Vector2f);
};


class Enemy : public Entity{
public:
	Enemy(Scene&, Vector2f, int, int, MoveDirection);
	void update(float) override;
	void mapProcessing();
};

class MovingPlatform : public Entity{
private:
	float timeToTurn;
public:
	MovingPlatform (Scene&, Vector2f, int, int, MoveDirection);
	void update(float) override;
	void mapProcessing();
};

class Coin : public Entity {
public:
	Coin (Scene&, Vector2f, int, int);
	void update(float) override;
};

class ExtraLife : public Entity {
private:
	int n_lives_to_add_ = 1;
public:
	ExtraLife(Scene&, Vector2f, int, int);
	void update(float) override;
};