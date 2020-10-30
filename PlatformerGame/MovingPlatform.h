#pragma once
#pragma once
#include "Entity.h"

class MovingPlatform : public Entity {
public:
	enum class _Type {

	};

public:
	MovingPlatform(Scene&, sf::Vector2f, int, int, entities::MoveDirection);
	MovingPlatform(Scene&, const TextureHolder&, sf::Vector2f, int, int, entities::MoveDirection);

	void update(float) override;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	void mapProcessing();

private:
	float time_to_turn_;
};