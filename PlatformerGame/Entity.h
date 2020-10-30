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

// TODO(me): Change update. Change entity to avoid storing vector of pointers to level objects.


class Entity : public SceneObject {
public:
	sf::Vector2f speed_;
	entities::MoveDirection direction_;
	entities::State state_;
	std::vector <std::shared_ptr<SceneObject>> level_objects_;
	entities::Type type_;
	float death_time_ = INFINITY;

public:
	Entity(Scene&, sf::Vector2f, int, int);
	Entity(Scene&, entities::Type, const TextureHolder&, sf::Vector2f, int, int);
	Entity();
	~Entity();

	virtual void update(float) = 0;
	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	entities::MoveDirection getDirection();

protected:
	sf::Sprite sprite_;
	AnimationPlayer animation_player_;
};

