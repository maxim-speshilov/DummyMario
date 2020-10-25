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
namespace entity {
	enum MoveDirection
	{
		kNowhere,
		kRight,
		kLeft,
		kUp,
		kDown
	};

	enum State
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

	enum Type
	{
		kPlayer,
		kEnemy,
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kExtraLife
	};


	class Entity {
	public:
		Vector2f speed_;
		FloatRect rect_;
		bool is_on_ground_, is_size_changed_;
		MoveDirection direction_;
		State state_;
		animation::AnimationEditor editor_;
		vector <std::shared_ptr<Object>> level_objects_;
		Type type_;
		float death_time_ = INFINITY;
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
		Vector2f key_speed_ = Vector2f(0.1f, 0.65f);
	public:
		Player(Scene&, Vector2f, int, int);
		bool is_on_platform_;
		std::map <String, bool> is_key_pressed_;
		void update(float) override;
		void mapProcessing(int);
		void keyboardProcessing();
		void setKeySpeed(Vector2f);
	};


	class Enemy : public Entity {
	public:
		Enemy(Scene&, Vector2f, int, int, MoveDirection);
		void update(float) override;
		void mapProcessing();
	};

	class MovingPlatform : public Entity {
	private:
		float time_to_turn_;
	public:
		MovingPlatform(Scene&, Vector2f, int, int, MoveDirection);
		void update(float) override;
		void mapProcessing();
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
}