#include "SceneObject.h"
#include "Utility.h"
#include <iostream>

SceneObject::SceneObject() {
}

void SceneObject::onCommand(const Command& command, float dt) {
	if (command.category & getCategory())
		command.action(*this, dt);
}

void SceneObject::move(float offset_x, float offset_y) {
	rect_.move(offset_x, offset_y);
}

void SceneObject::move(sf::Vector2f offset) {
	rect_.move(offset);
}

bool SceneObject::isDestroyed() const {
	return false;
}

bool SceneObject::isMarkedForRemoval() const {
	return isDestroyed();
}

sf::FloatRect SceneObject::getBoundingRect() const {
	return rect_.getGlobalBounds();
}

sf::Vector2f SceneObject::getPosition() const {
	return rect_.getPosition();
}

sf::Vector2f SceneObject::getSize() const {
	return rect_.getSize();
}

SceneObject::Type SceneObject::getType() const {
	return type_;
}

void SceneObject::setSize(float width, float height) {
	rect_.setSize(sf::Vector2f(width, height));
}

void SceneObject::setPosition(sf::Vector2f & pos) {
	rect_.setPosition(pos);
}

void SceneObject::setPosition(float x, float y) {
	rect_.setPosition(sf::Vector2f(x, y));
}

void SceneObject::setPositionX(float x) {
	rect_.setPosition(x, rect_.getPosition().y);
}

void SceneObject::setPositionY(float y) {
	rect_.setPosition(rect_.getPosition().x, y);
}

unsigned int SceneObject::getCategory() const {
	return (unsigned int)category::Type::SceneObject;
}

void SceneObject::update(float dt, CommandQueue& commands) {

}

void SceneObject::draw(sf::RenderTarget & rt, sf::RenderStates states) const {
	rt.draw(rect_, states);
}	

bool collision(const SceneObject& lhs, const SceneObject& rhs) {
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}