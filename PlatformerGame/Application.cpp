#include "Application.h"
#include "State.h"
#include "PreviewState.h"
#include "TitleState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "KeyBindingsState.h"
#include "BaseSettingsState.h"
#include "LevelLoadingState.h"
#include "SettingWorldState.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include <iostream>

Application::Application() :
	window_(sf::VideoMode(1920, 1080), "Test", sf::Style::Fullscreen),	
	textures_(),
	fonts_(),
	sound_player_(), 
	music_player_(),
	input_manager_(),
	settings_manager_(SettingsManager::Context(window_, sound_player_, music_player_, input_manager_)),
	state_stack_(State::Context(window_, textures_, fonts_, sound_player_, music_player_, input_manager_, settings_manager_)) {

	fonts_.load(Fonts::kMain, "fonts/SuperMario256.ttf");
	textures_.load(Textures::kButtonNormal, "textures/button_normal.png");

	registerStates();
	state_stack_.pushState(States::kMainMenu);
}

void Application::run() {
	sf::Clock clock;
	float time = 0.f;
	float time_per_frame_ = 1.f / 60.f;

	while (window_.isOpen()) {
		float dt = clock.restart().asSeconds();
		time += dt;
		while (time > time_per_frame_) {
			time -= time_per_frame_;

			processInput();
			update(time_per_frame_);

			if (state_stack_.isEmpty())
				window_.close();

		}

		render();
	}
}

void Application::processInput() {
	sf::Event event;

	while (window_.pollEvent(event)) {
		state_stack_.handleEvent(event);
		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Application::update(float dt) {
	state_stack_.update(dt);
}

void Application::render() {
	window_.clear();
	state_stack_.draw();
	window_.display();
}

void Application::registerStates() {

	state_stack_.registerState<PreviewState>(States::kPreview);
	state_stack_.registerState<TitleState>(States::kTitle);

	state_stack_.registerState<MainMenuState>(States::kMainMenu);
	state_stack_.registerState<SettingsState>(States::kSettings);
	state_stack_.registerState<KeyBindingsState>(States::kKeyBindings);
	state_stack_.registerState<GraphicsSettingsState>(States::kGraphics);
	state_stack_.registerState<AudioSettingsState>(States::kAudio);

	state_stack_.registerState<LevelLoadingState>(States::kLevelLoading);
	state_stack_.registerState<SettingWorldState>(States::kSettingWorld);

	state_stack_.registerState<GameState>(States::kGame);
	state_stack_.registerState<PauseState>(States::kPause);
	state_stack_.registerState<GameOverState>(States::kGameOver);
}
