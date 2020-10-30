#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	enum class Type {

	};

public: 
	Enemy(Scene&, Vector2f, int, int, Entity::MoveDirection);

	void update(float) override;
	void mapProcessing();

private:
	bool is_size_changed_;
};

