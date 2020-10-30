#pragma once
#include "Entity.h"

class MovingPlatform : public Entity {
public:
	enum class Type_ {

	};

public:
	MovingPlatform(Scene&, Vector2f, int, int, MoveDirection);

	void update(float) override;
	void mapProcessing();

private:
	float time_to_turn_;
};