#include "Music.h"

MusicPlayer::MusicPlayer() :
	music_(),
	music_id_(),
	filenames_map_(),
	volume_(100.f),
	is_looped_(false) {

	filenames_map_[Tracks::kMainTheme] = "music/MainTheme.ogg";
	filenames_map_[Tracks::kLostALife] = "music/LostALife.ogg";
	filenames_map_[Tracks::kGameOver] = "music/GameOver.ogg";
}

void MusicPlayer::play(Tracks music_id) {
	music_id_ = music_id;
	std::string  filename = filenames_map_[music_id];
	if (!music_.openFromFile(filename))
		throw std::runtime_error("music::MusicPlayer::play - Failed to open " + filenames_map_[music_id]);

	music_.setLoop(is_looped_);
	music_.setVolume(volume_);
	music_.play();
}

void MusicPlayer::pause() {
	music_.pause();
}

void MusicPlayer::stop() {
	music_.stop();
}

void MusicPlayer::setLoop(bool is_looped) {
	is_looped_ = is_looped;
	music_.setLoop(is_looped_);
}

void MusicPlayer::setVolume(float volume) {
	volume_ = volume;
	music_.setVolume(volume);
}


sf::SoundSource::Status MusicPlayer::getStatus(Tracks music_id) const {
	if (music_id == music_id_ && music_.getStatus() == sf::SoundSource::Playing)
		return sf::SoundSource::Playing;
	else
		return sf::SoundSource::Stopped;
}