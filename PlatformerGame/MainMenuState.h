#pragma once
#include "MenuState.h"

enum class MainMenuOptions {
	kPlay,
	kSettings,
	kExit
};

class MainMenuState : public MenuState<MainMenuOptions> {
public:
	MainMenuState(StateStack& stack, Context context);

private:
	void registerCallbacks() override;
};