#include "AnimationPlayer.h"

Textures toTextureID(SceneObject::Type type, Animation::ID id) {
	switch (type) {
	case SceneObject::Type::kPlayer:
		switch (id) {
		case Animation::ID::kStaying:
			return Textures::kRunningPlayer;
			break;
		case Animation::ID::kRunning:
			return Textures::kRunningPlayer;
			break;
		case Animation::ID::kJumping:
			return Textures::kJumpingPlayer;
			break;
		case Animation::ID::kRolling:
			return Textures::kRollingPlayer;
			break;
		default:
			break;
		}
		break;
	case SceneObject::Type::kEnemy:
		switch (id) {
		case Animation::ID::kRunning:
			return Textures::kEnemy;
			break;
		case Animation::ID::kDead:
			return Textures::kEnemy;
			break;
		default:
			break;
		}
		break;
	case SceneObject::Type::kBoomerang:
		switch (id) {
		case Animation::ID::kRunning:
			return Textures::kBoomerang;
			break;
		default:
			break;
		}
	case  SceneObject::Type::kCoin:
		switch (id) {
		case Animation::ID::kSpinning:
			return Textures::kCoin;
			break;
		default:
			break;
		}
		break;
	case SceneObject::Type::kExtraLife:
		switch (id) {
		case Animation::ID::kStaying:
			return Textures::kFullHeart;
			break;
		default:
			break;
		}
		break;
	case SceneObject::Type::kMovingPlatform:
		switch (id) {
		case Animation::ID::kRunning:
			return Textures::kMovingPlatform;
			break;
		default:
			break;
		}
		break;
	case  SceneObject::Type::kMovingVerticallyPlatform:
		switch (id) {
		case Animation::ID::kClimbing:
			return Textures::kMovingVerticallyPlatform;
			break;
		default:
			break;
		}
		break;
	}
}
	
AnimationPlayer::AnimationPlayer() {

}

AnimationPlayer::AnimationPlayer(SceneObject::Type type, const TextureHolder& texture_holder) {
	switch (type) {
	case SceneObject::Type::kPlayer:
		add(Animation::ID::kStaying, texture_holder.get(toTextureID(SceneObject::Type::kPlayer, Animation::ID::kStaying)), 0, 0, 23, 28, 1, 0.f, 0);
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(SceneObject::Type::kPlayer, Animation::ID::kRunning)), 0, 0, 23, 28, 8, 4.17f, 23);
		add(Animation::ID::kJumping, texture_holder.get(toTextureID(SceneObject::Type::kPlayer, Animation::ID::kJumping)), 0, 0, 24, 28, 4, 4.17f, 24);
		add(Animation::ID::kRolling, texture_holder.get(toTextureID(SceneObject::Type::kPlayer, Animation::ID::kRolling)), 0, 0, 22, 28, 8, 17.f, 22);
		break;
	case SceneObject::Type::kEnemy:
		add(Animation::ID::kDead, texture_holder.get(toTextureID(SceneObject::Type::kEnemy, Animation::ID::kDead)), 34, 33, 16, 8, 1, 0.f, 0);
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(SceneObject::Type::kEnemy, Animation::ID::kRunning)), 0, 25, 16, 16, 2, 4.17f, 16);
		break;
	case SceneObject::Type::kBoomerang:
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(SceneObject::Type::kBoomerang, Animation::ID::kRunning)), 0, 0, 16, 16, 4, 14.f, 16);
		break;
	case SceneObject::Type::kCoin:
		add(Animation::ID::kSpinning, texture_holder.get(toTextureID(SceneObject::Type::kCoin, Animation::ID::kSpinning)), 0, 0, 32, 32, 4, 6.66f, 32);
		break;
	case SceneObject::Type::kExtraLife:
		add(Animation::ID::kStaying, texture_holder.get(toTextureID(SceneObject::Type::kExtraLife, Animation::ID::kStaying)), 0, 0, 32, 32, 1, 0.f, 0);
		break;
	case SceneObject::Type::kMovingPlatform:
		add(Animation::ID::kRunning, texture_holder.get(toTextureID(SceneObject::Type::kMovingPlatform, Animation::ID::kRunning)), 0, 0, 96, 32, 1, 0.f, 0);
		break;
	case SceneObject::Type::kMovingVerticallyPlatform:
		add(Animation::ID::kClimbing, texture_holder.get(toTextureID(SceneObject::Type::kMovingVerticallyPlatform, Animation::ID::kClimbing)), 0, 0, 32, 32, 1, 0.f, 0);
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

bool AnimationPlayer::isReflex() {
	return animation_map_[key_id_].is_reflex_;
}

sf::Sprite AnimationPlayer::getSprite() {
	return animation_map_[key_id_].sprite_;
}

const sf::Texture* AnimationPlayer::getTexture() {
	return getSprite().getTexture();
}
