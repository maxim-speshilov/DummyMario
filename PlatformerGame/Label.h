#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Component.h"
#include "ResourceIdentifiers.h"

namespace gui {
class Label : public Component {
public:
	using Ptr = std::shared_ptr<Label>;

public:
	Label(const std::string& str, const FontHolder& fonts);
	Label(const sf::Text& text);

	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

	bool isSelectable() const override;

	virtual void centerOrigin();
	virtual void setString(const std::string& str);
	virtual void setText(const sf::Text& text);

	virtual sf::FloatRect getLocalBounds() const;
	virtual sf::FloatRect getGlobalBounds() const;
	virtual sf::Vector2f getSize() const;
	sf::Vector2f getPosition() const;

	void handleEvent(const sf::Event& event) override;

private:
	sf::Text text_;
};
}