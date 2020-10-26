#pragma once
#include "Entity.h"

// TODO(me): Fix position bug that happens when resizing window.
class Player : public Entity {
public:
	bool is_on_platform_;
	bool is_on_ground_;

	std::map <String, bool> is_key_pressed_;

public:
	Player(Scene&, Vector2f, int, int);
	Player();

	void update(float delta_time) override;

	void mapProcessing(int direction);
	void keyboardProcessing();

	void setKeySpeed(Vector2f key_speed);

private:
	Vector2f key_speed_ = Vector2f(0.1f, 0.65f);
};