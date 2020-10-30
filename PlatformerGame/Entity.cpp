#include "Entity.h"
#include "AnimationPlayer.h"

Entity::Entity(Scene& level, sf::Vector2f pos, int width, int height) {
	std::list <SceneObject> level_objects_ = level.getAllObjects();

	for (SceneObject object : level_objects_)
		this->level_objects_.push_back(std::make_shared<SceneObject>(object));	

	rect_.setPosition(pos);
	rect_.setSize(sf::Vector2f(width, height));
}

Entity::Entity(Scene& level, entities::Type type, const TextureHolder& textures, sf::Vector2f pos, int width, int height) : 
	animation_player_(type, textures) {
	std::list <SceneObject> level_objects_ = level.getAllObjects();

	for (SceneObject object : level_objects_)
		this->level_objects_.push_back(std::make_shared<SceneObject>(object));

	rect_.setPosition(pos);
	rect_.setSize(sf::Vector2f(width, height));
	type_ = type;
}


Entity::Entity() {

}

Entity::~Entity() { 

}

void Entity::draw(sf::RenderTarget &rt, sf::RenderStates states) const {
	rt.draw(rect_, states);
}

entities::MoveDirection Entity::getDirection() {
	return this->direction_;
}
