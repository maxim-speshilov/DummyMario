#pragma once
#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ColorIdentifiers.h"
#include <functional>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gui {
class Button : public Component {
public:
	using Ptr = std::shared_ptr<Button>;
	using Callback = std::function<void()>;

public:
	Button(const FontHolder& fonts);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	bool isSelectable() const override;
	void select() override;
	void deselect() override;

	void activate() override;
	void deactivate() override;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	void setCallback(Callback callback);
	void setString(const std::string& str);
	void setToggle(bool flag);

	void centerOrigin();
	void centerText();

	sf::Vector2f getSize() const override;
	sf::Vector2f getPosition() const override;

	void handleEvent(const sf::Event& event) override;

private:
	Callback callback_;

	sf::Color normal_color_;
	sf::Color selected_color_;
	sf::Color pressed_color_;
	sf::Texture normal_texture_;

	sf::Sprite sprite_;
	sf::Text text_;
	sf::Vector2f size_;

	bool is_toggle_;
};
}