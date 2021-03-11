#pragma once
#include "MenuState.h"

enum class PauseMenuOptions {
	kResume,
	kRestart,
	kMainMenu
};

class PauseState : public MenuState<PauseMenuOptions> {
public:
	PauseState(StateStack& stack, Context context);

	void draw() override;
	bool update(float dt) override;
	bool handleEvent(const sf::Event& event) override;
	
private:
	void registerCallbacks() override;
};