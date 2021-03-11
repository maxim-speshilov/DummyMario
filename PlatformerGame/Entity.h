#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <iterator>
#include <string>
#include "Scene.h"
#include "SceneObject.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "ResourceIdentifiers.h"

class Entity : public SceneObject {
public:
	using Direction = std::pair<entities::MoveDirection, entities::MoveDirection>;
public:
	sf::Vector2f speed_;
	entities::MoveDirection direction_;
	entities::State state_;

public:
	Entity(SceneObject::Type, const TextureHolder&);
	Entity();

	virtual void update(float dt, CommandQueue& commands) override;
	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	void updateDirection(); 

	entities::MoveDirection getDirection() const;
	entities::State getState() const;
	sf::Vector2f getSpeed() const;

	void setDirection(entities::MoveDirection dir);
	void setState(entities::State state);
	void setSpeed(const sf::Vector2f& speed);
	void setSpeed(float speed_x, float speed_y);
	void setSpeedX(float speed_x);
	void setSpeedY(float speed_y);

	void damage(float hp);
	void heal(float hp);

protected:
	sf::Sprite sprite_;
	AnimationPlayer animation_player_;

	float hp_;
};

