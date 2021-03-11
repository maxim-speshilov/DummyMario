#pragma once
#include "MenuState.h"

enum class SettingsMenuOptions {
	kControls,
	kGraphics,
	kAudio,
	kBack
};

class SettingsState : public MenuState<SettingsMenuOptions> {
public:
	SettingsState(StateStack& stack, Context context);

	void draw() override;
	bool update(float dt) override;
	bool handleEvent(const sf::Event& event) override;

private:
	void registerCallbacks() override;
};