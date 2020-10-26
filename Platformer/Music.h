#pragma once
#include <SFML/Audio.hpp>
#include "ResourceIdentifiers.h"
#include <map>

//TODO(me) : Add playlist implemetation.
class MusicPlayer : private sf::NonCopyable {
private:
	sf::Music music_;
	music::ID music_id_;
	std::map<music::ID, std::string> filenames_map_;
	float volume_;
	bool is_looped_;

public:
	MusicPlayer();

	void play(music::ID music_id);
	void pause();
	void stop();

	void setVolume(float volume);
	void setLoop(bool is_looped);

	sf::SoundSource::Status getStatus(music::ID sound_id) const;
};
