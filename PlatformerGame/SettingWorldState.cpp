#include "SettingWorldState.h"

SettingWorldState::SettingWorldState(StateStack& stack, Context context) : 
	State(stack, context),
	opacity_(150) {
}

void SettingWorldState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, opacity_));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);
}

bool SettingWorldState::update(float dt) {
	if (--opacity_ == 0) {
		requestStackPop();
	}
	return false;
}

bool SettingWorldState::handleEvent(const sf::Event& event) {
	return false;
}
