#pragma once
#include "State.h"

class StateStack;

class SettingWorldState : public State {
public:
	SettingWorldState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	int opacity_;
};