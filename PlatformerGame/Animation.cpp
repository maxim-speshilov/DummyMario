#include "Animation.h"

Animation::Animation() {}

Animation::~Animation() {
	frames_.clear();
	reflex_frames_.clear();
}

Animation::Animation(sf::Texture &texture, int left, int top, int width, int height, int number_of_frames, float animation_speed, int step) {
	current_frame_ = 0;
	animation_speed_ = animation_speed;

	is_reflex_ = false;
	is_on_ = true;

	sprite_.setTexture(texture);

	for (int i = 0; i < number_of_frames; i++) {
		frames_.push_back(sf::IntRect(left + i*step, top, width, height));
		reflex_frames_.push_back(sf::IntRect(left + i*step + width, top, -width, height));
	}
}

Animation::Animation(const sf::Texture &texture, int left, int top, int width, int height, int number_of_frames, float animation_speed, int step) {
	current_frame_ = 0;
	animation_speed_ = animation_speed;

	is_reflex_ = false;
	is_on_ = true;

	sprite_.setTexture(texture);

	for (int i = 0; i < number_of_frames; i++) {
		frames_.push_back(sf::IntRect(left + i * step, top, width, height));
		reflex_frames_.push_back(sf::IntRect(left + i * step + width, top, -width, height));
	}
}

void Animation::update(float dt) {
	if (!is_on_)
		return;

	current_frame_ += animation_speed_ * dt;

	if (current_frame_ > frames_.size())
		current_frame_ -= (float)frames_.size();
	if (!is_reflex_)
		sprite_.setTextureRect(frames_[(int)current_frame_]);
	else
		sprite_.setTextureRect(reflex_frames_[(int)current_frame_]);
}

void Animation::update(sf::RectangleShape &rect, float dt) {
	if (!is_on_)
		return;

	rect.setTexture(sprite_.getTexture(), true);

	current_frame_ += animation_speed_ * dt;

	if (current_frame_ > frames_.size())
		current_frame_ -= (float)frames_.size();
	if (!is_reflex_)
		rect.setTextureRect(frames_[(int)current_frame_]);
	else
		rect.setTextureRect(reflex_frames_[(int)current_frame_]);
}

