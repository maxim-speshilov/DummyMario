#include "Animation.h"

using namespace animation;

Animation::Animation() {}

Animation::~Animation() {
	frames_.clear();
	reflex_frames_.clear();
}

Animation::Animation(Texture &texture, int left, int top, int width, int height, int number_of_frames, float animation_speed, int step) {
	current_frame_ = 0;
	animation_speed_ = animation_speed;

	is_reflex_ = false;
	is_on_ = true;

	sprite_.setTexture(texture);

	for (int i = 0; i < number_of_frames; i++) {
		frames_.push_back(IntRect(left + i*step, top, width, height));
		reflex_frames_.push_back(IntRect(left + i*step + width, top, -width, height));
	}
}

void Animation::shift(float elapsed_time) {
	if (!is_on_)
		return;

	current_frame_ += animation_speed_ * elapsed_time;

	if (current_frame_ > frames_.size())
		current_frame_ -= (float)frames_.size();
	if (!is_reflex_)
		sprite_.setTextureRect(frames_[(int)current_frame_]);
	else
		sprite_.setTextureRect(reflex_frames_[(int)current_frame_]);
}

void AnimationEditor::addAnimation(ID id, Texture &texture, int left, int top, int width, int height, int number_of_frames, float animation_speed, int step) {
	animation_map_[id] = Animation(texture, left, top, width, height, number_of_frames, animation_speed, step);
	key_id_ = id;
}

void AnimationEditor::setAnimation(ID id) {
	key_id_ = id;
}

void AnimationEditor::playAnimation() {
	animation_map_[key_id_].is_on_ = true;
}

void AnimationEditor::pauseAnimation() {
	animation_map_[key_id_].is_on_ = false;
}

void AnimationEditor::setReflex(bool boo){
	animation_map_[key_id_].is_reflex_ = boo;
}

void AnimationEditor::shiftAnimation(float elapsedTime){
	animation_map_[key_id_].shift(elapsedTime);
}

void AnimationEditor::drawAnimation(RenderWindow &window, int x, int y){
	animation_map_[key_id_].sprite_.setPosition(x, y);
	window.draw(animation_map_[key_id_].sprite_);
}

void AnimationEditor::drawAnimation(RenderTexture &rt, int x, int y){
	animation_map_[key_id_].sprite_.setPosition(x, y);
	rt.draw(animation_map_[key_id_].sprite_);
}

void AnimationEditor::drawAnimationByID(ID key_id, RenderTexture &rt, int x, int y) {
	animation_map_[key_id].sprite_.setPosition(x, y);
	rt.draw(animation_map_[key_id].sprite_);
}