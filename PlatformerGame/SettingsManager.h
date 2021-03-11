#pragma once
#include "SettingsIdentifiers.h"
#include "Music.h"
#include "Sound.h"
#include "InputManager.h"
#include "magic_enum.hpp"
#include <functional>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class SettingsManager : private sf::NonCopyable {
public:
	using SettingsTable = std::array<std::vector<size_t>, magic_enum::enum_count<Settings>()>;
	using CallbacksTable = std::array<std::vector<std::function<void()>>,
		magic_enum::enum_count<Settings>()>;

public:
	struct Context {
		Context();
		Context(sf::RenderWindow& window, SoundPlayer& sound_player, 
			MusicPlayer& music_player, InputManager& input_manager);

		sf::RenderWindow* window;
		SoundPlayer* sound_player;
		MusicPlayer* music_player;
		InputManager* input_manager;
	};

public:

	SettingsManager();
	SettingsManager(Context context);

	size_t getSetting(std::size_t setting_type, std::size_t setting);
	void getSettingsFromXml(const std::string& filename);

	void saveToXml(const std::string& filename);
	void set(std::size_t setting_type, std::size_t setting, std::size_t value);
	void setContext(Context context);

	void registerCallbacks();
	void registerSettings();

private:
	Context context_;

	SettingsTable settings_;
	CallbacksTable callbacks_;
};