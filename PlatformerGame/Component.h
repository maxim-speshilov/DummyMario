#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace gui {
class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
public:
	using Ptr = std::shared_ptr<Component>;

public:
	Component();

	virtual bool isSelectable() const = 0;

	bool isSelected() const;
	virtual void select();
	virtual void deselect();

	virtual void move(const sf::Vector2f& offset);
	virtual bool isActive() const;
	virtual void activate();
	virtual void deactivate();

	virtual sf::Vector2f getSize() const = 0;
	virtual sf::Vector2f getPosition() const;
	virtual sf::FloatRect getLocalBounds() const = 0;
	virtual sf::FloatRect getGlobalBounds() const;

	virtual void handleEvent(const sf::Event& event) = 0;

private:
	bool is_selected_;
	bool is_active_;
};
}