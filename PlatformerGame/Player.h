#pragma once
#include "Entity.h"
#include <map>
#include <SFML/System/String.hpp>

class Player : public Entity {
public:
	bool is_on_platform_;
	bool is_on_ground_;
	bool is_throwing_boomerang_;

public:
	Player(const TextureHolder& textures);
	Player(SceneObject::Type, const TextureHolder& textures);
	Player();

	void update(float dt, CommandQueue& commands) override;

	void damage(int hp);
	void heal(int hp);
	void addScore(int sc_to_add);
	void makeInvulnerable();
	void throwBoomerang();

	void checkThrowingBoomerang(float dt, CommandQueue& commands);
	void checkIsInvulnerable(float dt, CommandQueue& commands);

	bool isThrowing() const;
	int getBoomerangsLeft() const;
	int getNumberOfLifes() const;
	int getMaxNumberOfLifes() const;
	int getScore() const;
	unsigned int getCategory() const;

	void setKeySpeed(sf::Vector2f key_speed);

private:
	sf::Vector2f key_speed_;
	int score_;
	int n_of_lifes_;
	int max_n_of_lifes_;

	bool is_invulnerable_;
	float invulnerable_effect_time_;

	int boomerangs_left_;
	float cooldown_time_;
	Command throw_boomerang_;
};