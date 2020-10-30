#pragma once
#include <SFML/Audio.hpp>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include <map>
#include <utility>
#include <algorithm>
#include <list>


class SoundPlayer : private sf::NonCopyable {
private:
	SoundBufferHolder sound_buffers_;
	std::list<sf::Sound> sounds_;
	float volume_;

public:
	SoundPlayer();

	void play(sounds::ID sound_id);

	void removeStoppedSounds();

	void setVolume(float volume);

	sf::SoundSource::Status getStatus(const sounds::ID sound_id) const;
};
