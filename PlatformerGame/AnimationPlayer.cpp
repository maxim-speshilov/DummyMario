#include "AnimationPlayer.h"

textures::ID toTextureID(entities::Type type, Animation::ID id) {
	switch (type) {
	case entities::Type::kPlayer:
		switch (id) {
		case Animation::ID::kStaying:
			return textures::ID::kRunningPlayer;
			break;
		case Animation::ID::kRunning:
			return textures::ID::kRunningPlayer;
			break;
		case Animation::ID::kJumping:
			return textures::ID::kJumpingPlayer;
			break;
		case Animation::ID::kRolling:
			return textures::ID::kRollingPlayer;
			break;
		default:
			break;
		}
		break;
	case entities::Type::kEnemy:
		switch (id) {
		case Animation::ID::kRunning:
			return textures::ID::kEnemy;
			break;
		case Animation::ID::kDead:
			return textures::ID::kEnemy;
			break;
		default:
			break;
		}
		break;
	case  entities::Type::kCoin:
		switch (id) {
		case Animation::ID::kSpinning:
			return textures::ID::kCoin;
			break;
		default:
			break;
		}
		break;
	case entities::Type::kExtraLife:
		switch (id) {
		case Animation::ID::kStaying:
			return textures::ID::kFullHeart;
			break;
		default:
			break;
		}
		break;
	case entities::Type::kMovingPlatform:
		switch (id) {
		case Animation::ID::kRunning:
			return textures::ID::kMovingPlatform;
			break;
		default:
			break;
		}
		break;
	case  entities::Type::kMovingVerticallyPlatform:
		switch (id) {
		case Animation::ID::kClimbing:
			return textures::ID::kMovingVerticallyPlatform;
			break;
		default:
			break;
		}
		break;
	}
}
	
AnimationPlayer::AnimationPlayer() {

}

AnimationPlayer::AnimationPlayer(entities::Type type, const TextureHolder& texture_holder) {
	switch (type) {
	case entities::Type::kPlayer:
		add(Animation::ID::kStaying, texture_holder.get(toTextureID(entities::Type::kPlayer, Animation::ID::kStaying)), 0, 0, 23, 28, 1, 0, 0);
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(entities::Type::kPlayer, Animation::ID::kRunning)), 0, 0, 23, 28, 8, 0.005, 23);
		add(Animation::ID::kJumping, texture_holder.get(toTextureID(entities::Type::kPlayer, Animation::ID::kJumping)), 0, 0, 24, 28, 4, 0.005, 24);
		add(Animation::ID::kRolling, texture_holder.get(toTextureID(entities::Type::kPlayer, Animation::ID::kRolling)), 0, 0, 22, 28, 8, 0.03, 22);
		break;
	case entities::Type::kEnemy:
		add(Animation::ID::kDead, texture_holder.get(toTextureID(entities::Type::kEnemy, Animation::ID::kDead)), 34, 33, 16, 8, 1, 0, 0);
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(entities::Type::kEnemy, Animation::ID::kRunning)), 0, 25, 16, 16, 2, 0.005, 16);
		break;
	case entities::Type::kCoin:
		add(Animation::ID::kSpinning, texture_holder.get(toTextureID(entities::Type::kCoin, Animation::ID::kSpinning)), 0, 0, 32, 32, 4, 0.008, 32);
		break;
	case entities::Type::kExtraLife:
		add(Animation::ID::kStaying, texture_holder.get(toTextureID(entities::Type::kExtraLife, Animation::ID::kStaying)), 0, 0, 32, 32, 1, 0, 0);
		break;
	case entities::Type::kMovingPlatform:
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(entities::Type::kMovingPlatform, Animation::ID::kRunning)), 0, 0, 96, 32, 1, 0, 0);
		break;
	case entities::Type::kMovingVerticallyPlatform:
		add(Animation::ID::kClimbing, texture_holder.get(toTextureID(entities::Type::kMovingVerticallyPlatform, Animation::ID::kClimbing)), 0, 0, 32, 32, 1, 0, 0);
		break;
	default:
		break;
	}
}

void AnimationPlayer::add(Animation::ID id, sf::Texture &texture, int left, int top, int width, int height,
	int number_of_frames, float animation_speed, int step) {
	animation_map_[id] = Animation(texture, left, top, width, height, number_of_frames, animation_speed, step);
	key_id_ = id;
}

void AnimationPlayer::add(Animation::ID id, const sf::Texture &texture, int left, int top, int width, int height,
	int number_of_frames, float animation_speed, int step) {
	animation_map_[id] = Animation(texture, left, top, width, height, number_of_frames, animation_speed, step);
	key_id_ = id;
}

void AnimationPlayer::set(Animation::ID id) {
	key_id_ = id;
}

void AnimationPlayer::play() {
	animation_map_[key_id_].is_on_ = true;
}

void AnimationPlayer::pause() {
	animation_map_[key_id_].is_on_ = false;
}

void AnimationPlayer::update(float dt) {
	animation_map_[key_id_].update(dt);
}

void AnimationPlayer::update(sf::RectangleShape &rect, float dt) {
	animation_map_[key_id_].update(rect, dt);
}

void AnimationPlayer::setReflex(bool boo) {
	animation_map_[key_id_].is_reflex_ = boo;
}

sf::Sprite AnimationPlayer::getSprite() {
	return animation_map_[key_id_].sprite_;
}

const sf::Texture* AnimationPlayer::getTexture() {
	return getSprite().getTexture();
}
