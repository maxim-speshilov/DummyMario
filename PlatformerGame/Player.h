#pragma once
#include "Entity.h"
#include <map>
#include <SFML/System/String.hpp>

class Player : public Entity {
public:
	bool is_on_platform_;
	bool is_on_ground_;

	std::map <sf::String, bool> is_key_pressed_;

public:
	Player(Scene&, sf::Vector2f, int, int);
	Player(Scene&, const TextureHolder&, sf::Vector2f, int, int);
	Player();

	void update(float delta_time) override;

	void mapProcessing(int direction);
	void keyboardProcessing();

	void setKeySpeed(sf::Vector2f key_speed);

private:
	sf::Vector2f key_speed_ = sf::Vector2f(0.1f, 0.65f);
};