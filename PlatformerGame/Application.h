#pragma once
#include <SFML/System/NonCopyable.hpp>
#include "StateStack.h"
#include "SettingsManager.h"


class Application : private sf::NonCopyable {
public:
	Application();

	void run();

private:
	void processInput();
	void update(float dt);
	void render();

	void registerStates();

private:
	sf::RenderWindow window_;
	TextureHolder textures_;
	FontHolder fonts_;
	SoundPlayer sound_player_;
	MusicPlayer music_player_;
	InputManager input_manager_;
	SettingsManager settings_manager_;
	StateStack state_stack_;
};