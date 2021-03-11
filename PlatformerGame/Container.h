#pragma once
#include "Component.h"
#include <SFML/Graphics/Rect.hpp>
#include <vector>

namespace gui {
class Container : public Component {
public:
	using Ptr = std::shared_ptr<Container>;
	using ComponentVector = std::vector<Component::Ptr>;

public:
	Container();

	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	void pack(Component::Ptr component);

	bool isSelectable() const override;
	bool hasSelected() const;
	void select(unsigned int index);
	void selectNext();
	void selectPrevious();
	
	void centerOrigin();

	sf::FloatRect getLocalBounds() const override;
	sf::FloatRect getGlobalBounds() const override; 
	sf::Vector2f getSize() const override;
	sf::Vector2f getPosition() const;
	sf::Vector2f getCenter() const;

	void setPosition(const sf::Vector2f& position);

	void handleEvent(const sf::Event& event) override;

private:
	ComponentVector children_;
	
	int selected_index_;
};
}