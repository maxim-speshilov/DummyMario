#include "PauseState.h"
#include "Utility.h"
#include "ColorIdentifiers.h"

PauseState::PauseState(StateStack& stack, Context context) :
	MenuState(stack, context) {

	registerCallbacks();
	createContainer();

	context.music_player->pause();
}

void PauseState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, 150));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);
	
	window.draw(gui_container_);
}

bool PauseState::update(float dt) {
	return false;
}

bool PauseState::handleEvent(const sf::Event& event) {
	gui_container_.handleEvent(event);
	return false;
}

void PauseState::registerCallbacks() {
	callback_map_[PauseMenuOptions::kResume] = [this]() {
		requestStackPop();
	};

	callback_map_[PauseMenuOptions::kRestart] = [this]() {
		requestStackClear();
		requestStackPush(States::kGame);
		requestStackPush(States::kSettingWorld);
	};

	callback_map_[PauseMenuOptions::kMainMenu] = [this]() {
		requestStackClear();
		requestStackPush(States::kMainMenu);
	};
}
