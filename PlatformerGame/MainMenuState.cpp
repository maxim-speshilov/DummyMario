#include "MainMenuState.h"

MainMenuState::MainMenuState(StateStack& stack, Context context) :
	MenuState(stack, context) {

	registerCallbacks();
	createContainer();

	context.textures->clear();
}

void MainMenuState::registerCallbacks() {
	callback_map_[MainMenuOptions::kPlay] =  [this]() {
		requestStackPop();
		requestStackPush(States::kLevelLoading);
	};

	callback_map_[MainMenuOptions::kSettings] = [this]() {
		requestStackPush(States::kSettings);
	};

	callback_map_[MainMenuOptions::kExit] = [this]() {
		requestStackPop();
	};
}
