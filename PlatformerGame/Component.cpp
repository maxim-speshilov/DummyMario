#include "Component.h"

using namespace gui;

Component::Component() :
	is_selected_(false),
	is_active_(false) {
}

bool Component::isSelected() const {
	return is_selected_;
}

void Component::select() {
	is_selected_ = true;
}

void Component::deselect() {
	is_selected_ = false;
}

void gui::Component::move(const sf::Vector2f& offset) {
	Transformable::move(offset);
}

bool Component::isActive() const {
	return is_active_;
}

void Component::activate() {
	is_active_ = true;
}

void Component::deactivate() {
	is_active_ = false;
}

sf::Vector2f gui::Component::getPosition() const {
	return Transformable::getPosition();
}

sf::FloatRect gui::Component::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}
