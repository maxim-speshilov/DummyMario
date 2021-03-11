#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceIdentifiers.h"
#include "StateIdentifiers.h"
#include "InputManager.h"
#include "SettingsManager.h"
#include "Music.h"
#include "Sound.h"

class StateStack;

class State {
public:
	struct Context {
	public:
		Context();
		Context(sf::RenderWindow& window, TextureHolder& textures,
				FontHolder& fonts, SoundPlayer& sound_player, 
				MusicPlayer& music_player, InputManager& input_manager,
				SettingsManager& settings_manager);
	
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		SoundPlayer* sound_player;
		MusicPlayer* music_player;
		InputManager* input_manager;
		SettingsManager* settings_manager;
	};

public:
	State(StateStack& stack, Context context);

	virtual void draw() = 0;
	virtual bool update(float dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States state_id);
	void requestStackPop();
	void requestStackClear();

	Context getContext() const;

protected:
	StateStack* stack_;
	Context context_;
};