#include "Container.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include <cassert>

using namespace gui;

Container::Container() :
	children_(), 
	selected_index_(-1) {
}

void gui::Container::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();

	for (auto& child : children_)
		rt.draw(*child, states);

}

void Container::pack(Component::Ptr component) {
	children_.push_back(component);

	if (!hasSelected() && component->isSelectable())
		select(children_.size() - 1);
}

bool Container::isSelectable() const {
	return false;
}

bool Container::hasSelected() const {
	return selected_index_ >= 0;
}

void Container::select(unsigned int index) {
	if (children_[index]->isSelectable()) {
		if (hasSelected())
			children_[selected_index_]->deselect();

		children_[index]->select();
		selected_index_ = index;
	}
}

void Container::selectNext() {
	if (!hasSelected())
		return; 
	
	int next = selected_index_;

	do
		++next %= children_.size();
	while (!children_[next]->isSelectable());

	select(next);
}

void Container::selectPrevious() {
	if (!hasSelected())
		return;

	int prev = selected_index_;

	do
		prev = (prev + children_.size() - 1) % children_.size();
	while (!children_[prev]->isSelectable());

	select(prev);
}

void gui::Container::centerOrigin() {
	auto bounds = getLocalBounds();
	setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

sf::FloatRect gui::Container::getLocalBounds() const
{
	return sf::FloatRect();
}

// TODO(me) : Optimize.

sf::FloatRect gui::Container::getGlobalBounds() const {

	auto bottom_it = std::max_element(children_.begin(), children_.end(), [](const Component::Ptr& lhs, const Component::Ptr& rhs) {
		return (lhs->getPosition().y + lhs->getSize().y < rhs->getPosition().y + rhs->getSize().y); 
	});

	auto top_it = std::min_element(children_.begin(), children_.end(), [](const Component::Ptr& lhs, const Component::Ptr& rhs) {
		return (lhs->getPosition().y < rhs->getPosition().y);
		});

	auto left_it = std::min_element(children_.begin(), children_.end(), [](const Component::Ptr& lhs, const Component::Ptr& rhs) {
		return (lhs->getPosition().x < rhs->getPosition().x); 
	});

	auto right_it = std::max_element(children_.begin(), children_.end(), [](const Component::Ptr& lhs, const Component::Ptr& rhs) {
		return (lhs->getPosition().x + lhs->getSize().x < rhs->getPosition().x + rhs->getSize().x);
		});

	float left = (*left_it)->getPosition().x;
	float right = (*right_it)->getPosition().x + (*right_it)->getSize().x;
	float top = (*top_it)->getPosition().y;
	float bottom = (*bottom_it)->getPosition().y + (*bottom_it)->getSize().y;

	return sf::FloatRect(left, top, right - left, bottom - top);
}

sf::Vector2f gui::Container::getSize() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::Vector2f gui::Container::getPosition() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.left, bounds.top);
}

sf::Vector2f Container::getCenter() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void gui::Container::setPosition(const sf::Vector2f& position) {
	auto offset = position - getCenter();
	for (auto& child : children_)
		child->move(offset);
}

void Container::handleEvent(const sf::Event& event) {
	if (hasSelected() && children_[selected_index_]->isActive()) {
		children_[selected_index_]->handleEvent(event);
	} else if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Up:
			selectPrevious();
			break;
		case sf::Keyboard::Down:
			selectNext();
			break;
		case sf::Keyboard::Return:
			if (hasSelected())
				children_[selected_index_]->activate();
			break;
		default:
			break;
		}
	}
}

