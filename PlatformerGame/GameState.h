#pragma once
#include "World.h"
#include "InputManager.h" 


class GameState : public State {
public:
	GameState(StateStack& stack, Context context);

	bool update(float dt) override;
	void draw() override;
	bool handleEvent(const sf::Event& event) override;

private:
	World world_;
	InputManager& input_manager_;
	bool is_paused_;
};