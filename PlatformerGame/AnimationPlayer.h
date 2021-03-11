#pragma once
#include "Animation.h" 
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "EntityIdentifiers.h"
#include <map>
#include <SFML/Graphics/RectangleShape.hpp>
#include "SceneObject.h"


class AnimationPlayer {
private:
	Animation::ID key_id_;
	std::map <Animation::ID, Animation> animation_map_;

public:
	AnimationPlayer(SceneObject::Type type, const TextureHolder& texture_holder);
	AnimationPlayer();
	~AnimationPlayer() {}

	void add(Animation::ID, sf::Texture&, int, int, int, int, int, float, int);
	void add(Animation::ID, const sf::Texture&, int, int, int, int, int, float, int);
	void play();
	void pause();
	void update(float dt);
	void update(sf::RectangleShape& rect, float dt);

	void set(Animation::ID);
	void setReflex(bool);

	bool isReflex();
	sf::Sprite getSprite();
	const sf::Texture* getTexture();
};