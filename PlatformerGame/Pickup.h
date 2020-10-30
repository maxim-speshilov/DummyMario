#pragma once
#include "Entity.h"

class Pickup : public Entity {
public:
	enum class _Type {
		_kCoin,
		_kExtraLife
	};

public:
	Pickup(entities::Type type, Scene& scene, sf::Vector2f pos, int width, int height);
	Pickup(Scene& scene, entities::Type type, const TextureHolder&, sf::Vector2f pos, int width, int height);

	void update(float) override;

public:
	_Type _type_;

};