#pragma once
#include "State.h"

class StateStack;

class GameOverState : public State {
public:
	enum class Options {
		kPlayAgain,
		kMainMenu
	};

public:
	GameOverState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	std::vector<sf::Text> options_;
	unsigned int option_index_;
	float time_;
	sf::Text game_over_text_;
	bool show_game_over_;
};