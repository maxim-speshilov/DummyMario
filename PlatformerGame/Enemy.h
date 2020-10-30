#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	enum class _Type {

	};

public: 
	Enemy(Scene&, sf::Vector2f, int, int, entities::MoveDirection);
	Enemy(Scene&, const TextureHolder&, sf::Vector2f, int, int, entities::MoveDirection);
	void update(float) override;
	void mapProcessing();

private:
	bool is_size_changed_;
};

