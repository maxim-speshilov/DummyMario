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

// TODO(me): Change update. Change entity to avoid storing vector of pointers to level objects.

class Entity {
public:
	enum class MoveDirection
	{
		kNowhere,
		kRight,
		kLeft,
		kUp,
		kDown
	};

	enum class State
	{
		kStaying,
		kRunning,
		kJumping,
		kRolling,
		kSwimming,
		kClimbing,
		kSliding,
		kDead,
		kInvulnerable
	};

	enum class Type
	{
		kPlayer,
		kEnemy,
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kExtraLife
	};

public:
	Vector2f speed_;
	FloatRect rect_;
	MoveDirection direction_;
	State state_;
	animation::AnimationEditor editor_;
	vector <std::shared_ptr<Object>> level_objects_;
	Type type_;
	float death_time_ = INFINITY;

public:
	Entity(Scene&, Vector2f, int, int);
	Entity();
	~Entity();

	virtual void update(float) = 0;

	MoveDirection getDirection();
};

class Coin : public Entity {
public:
	Coin(Scene&, Vector2f, int, int);
	void update(float) override;
};

class ExtraLife : public Entity {
private:
	int n_lives_to_add_ = 1;
public:
	ExtraLife(Scene&, Vector2f, int, int);
	void update(float) override;
};
