#include "State.h"
#include "StateStack.h"
#include <iostream>

State::Context::Context() :
	window(),
	textures(),
	fonts(),
	sound_player(),
	music_player(),
	input_manager(),
	settings_manager() {
}

State::Context::Context(sf::RenderWindow& window_, TextureHolder& textures_, FontHolder& fonts_,
	SoundPlayer& sound_player_, MusicPlayer& music_player_, InputManager& input_manager_, SettingsManager& settings_manager_) :
	window(&window_),
	textures(&textures_),
	fonts(&fonts_),
	sound_player(&sound_player_),
	music_player(&music_player_),
	input_manager(&input_manager_),
	settings_manager(&settings_manager_) {
}

State::State(StateStack& stack, Context context) :
	stack_(&stack),
	context_(context) {
}

void State::requestStackPush(States state_id) {
	stack_->pushState(state_id);
}

void State::requestStackPop() {
	stack_->popState();
}

void State::requestStackClear() {
	stack_->clearStates();
}

State::Context State::getContext() const {
	return context_;
}
