#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <SFML\Graphics.hpp>
#include <string>
#include <SFML\Audio.hpp>
#include "Animation.h"
#include "Level.h"

using namespace sf;

class Entity{
protected:
	Vector2f speed;
	float currentFrame;
	FloatRect rect;
	bool isOnGround, isSizeChanged;
	enum MoveDirection { Right, Left } direction;
	enum EntityState { Staying, Running, Jumping, Rolling, Swimming, Climbing, Dead} state;
	AnimationEditor editor;
	vector <Object> levelObjects;
	string type;
public:
	Entity(Level&, Vector2f, int, int);
	virtual void update(float) = 0;
	friend bool gameLoop();
};

class Player : public Entity {
public:
	Player(Level&, Vector2f, int, int);
	bool isWounded, isOnPlatform;
	std::map <String, bool> isKeyPressed;
	void update(float) override;
	void mapProcessing(int);
	void keyboardWork();
};

class Enemy : public Entity{
public:
	Enemy(Level&, Vector2f, int, int, MoveDirection);
	void update(float) override;
	void mapProcessing();
};

class MovingPlatform : public Entity{
private:
	float timeToTurn;
public:
	MovingPlatform (Level&, Vector2f, int, int);
	void update(float);
};
