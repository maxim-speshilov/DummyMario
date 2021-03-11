#include "GameState.h"
#include "StateStack.h"
#include "Utility.h"
#include "OptionShiftingLabel.h"
#include <chrono>

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),
	world_(context),
	input_manager_(*context.input_manager),
	is_paused_(false) {

	getContext().music_player->play(Tracks::kMainTheme);
	getContext().music_player->setLoop(true);

}

void GameState::draw() {
	world_.draw();
}

bool GameState::update(float dt) {
	world_.update(dt);
	
	if (world_.isPlayerDead())
		requestStackPush(States::kGameOver);

	CommandQueue& commands = world_.getCommandQueue();
	input_manager_.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event) {
	
	CommandQueue& commands = world_.getCommandQueue();
	input_manager_.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) 
		requestStackPush(States::kPause);
	
	return true;
}
