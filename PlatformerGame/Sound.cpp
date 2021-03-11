#include "Sound.h"


SoundPlayer::SoundPlayer() :
	sound_buffers_(),
	sounds_(),
	volume_(100.f) {
	sound_buffers_.load(Sounds::kJump, "sounds/Jump.ogg");
	sound_buffers_.load(Sounds::kPickedACoin, "sounds/Coin.ogg");
}

void SoundPlayer::play(Sounds sound_id) {
	sf::Sound sound;
	sound.setBuffer(sound_buffers_.get(sound_id));
	sound.setVolume(volume_);
	sounds_.push_back(sound);
	sounds_.back().play();
}

void SoundPlayer::removeStoppedSounds() {
	sounds_.remove_if([] (const sf::Sound& sound) {
		return sound.getStatus() == sf::Sound::Stopped;
	});

}

void SoundPlayer::setVolume(float volume) {
	volume_ = volume;
}

sf::SoundSource::Status SoundPlayer::getStatus(const Sounds sound_id) const {
	return sf::SoundSource::Status();
}

