#include "SceneObject.h"

SceneObject::SceneObject() {
}

std::string SceneObject::getPropertyByName(std::string name) const {
	return properties_.at(name);
}

std::map <std::string, std::string> SceneObject::getProperties() const {
	return properties_;
}

void SceneObject::update(float dt) const {

}

void SceneObject::draw(sf::RenderTarget & rt, sf::RenderStates states) const {
	
}

void setPositionX(sf::Transformable &object, float x) {
	object.setPosition(x, object.getPosition().y);
}

void setPositionY(sf::Transformable &object, float y) {
	object.setPosition(object.getPosition().x, y);
}