#include "SettingsState.h"

SettingsState::SettingsState(StateStack& stack, Context context) :
	MenuState(stack, context) {

	registerCallbacks();
	createContainer();
}

void SettingsState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, 255));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);

	window.draw(gui_container_);
}

bool SettingsState::update(float dt) {
	return false;
}

bool SettingsState::handleEvent(const sf::Event& event) {
	gui_container_.handleEvent(event);
	return false;
}

void SettingsState::registerCallbacks() {
	callback_map_[SettingsMenuOptions::kControls] = [this]() {
		requestStackPush(States::kKeyBindings);
	};

	callback_map_[SettingsMenuOptions::kGraphics] = [this]() {
		requestStackPush(States::kGraphics);
	};

	callback_map_[SettingsMenuOptions::kAudio] = [this]() {
		requestStackPush(States::kAudio);
	};


	callback_map_[SettingsMenuOptions::kBack] = [this]() {
		requestStackPop();
	};
}

