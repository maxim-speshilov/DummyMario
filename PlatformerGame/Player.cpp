#include "Player.h"
#include "Boomerang.h"
#include "UIWidget.h"

Player::Player(const TextureHolder &textures) :
	Entity(SceneObject::Type::kPlayer, textures),
	score_(0),
	n_of_lifes_(3),
	max_n_of_lifes_(3),
	is_throwing_boomerang_(false),
	cooldown_time_(0.f),
	is_invulnerable_(false),
	key_speed_(0.1f, 0.65f) {
}

Player::Player(SceneObject::Type type, const TextureHolder &textures) :
	Entity(SceneObject::Type::kPlayer, textures),
	score_(0),
	n_of_lifes_(3),
	max_n_of_lifes_(3),
	is_throwing_boomerang_(false),
	is_invulnerable_(false),
	invulnerable_effect_time_(0.f),
	key_speed_(0.1f, 0.65f),
	boomerangs_left_(10) {
	
	assert(type == SceneObject::Type::kPlayer);
	is_on_ground_ = true;
	rect_.setSize(sf::Vector2f(23, 28));
	rect_.setTexture(&textures.get(Textures::kRunningPlayer));
	rect_.setTextureRect(sf::IntRect(0, 0, 23, 28));
	state_ = entities::State::kStaying;

	throw_boomerang_.action = derivedAction<SceneRoot>([this, &textures](SceneRoot& root, float dt) {
		auto boomerang = std::make_unique<Boomerang>(textures);
		int sign = (!animation_player_.isReflex()) ? 1 : -1;
		float k = (float)sign / 2.f + 1.f / 2.f;
		boomerang->setPosition(getPosition().x + k * getSize().x - 4.f + 8 * (k - 1), getPosition().y + 2.f);
		boomerang->setSpeedX(sign * boomerang->getSpeed().x);
		root.scene_->addObjectToGroup(Scene::ObjectGroup::Type::Entities, std::move(boomerang));
		});

	throw_boomerang_.category = static_cast<unsigned int>(category::Type::Scene);
}

Player::Player() :
	Entity() {
	state_ = entities::State::kStaying;
	type_ = SceneObject::Type::kPlayer;
	is_on_ground_ = true;
	speed_ = sf::Vector2f(0, 0);
	rect_.setFillColor(sf::Color::White);
	rect_.setSize(sf::Vector2f(32.f, 32.f));
}

void Player::update(float dt, CommandQueue& commands) {
	if (!is_on_ground_)
		state_ = entities::State::kJumping;

	animation_player_.set(magic_enum::enum_cast<Animation::ID>(magic_enum::enum_name<entities::State>(state_)).value());

	if (direction_ == entities::MoveDirection::kLeft)
		animation_player_.setReflex(true);
	if (direction_ == entities::MoveDirection::kRight)
		animation_player_.setReflex(false);

	if (cooldown_time_ > 0.f)
		cooldown_time_ -= dt;

	if (cooldown_time_ < 0.f)
		cooldown_time_ = 0.f;

	checkThrowingBoomerang(dt, commands);
	checkIsInvulnerable(dt, commands);

	speed_.y += 1300.f * dt;
	rect_.move(speed_ * dt);
	animation_player_.update(rect_, dt);
	is_on_ground_ = false;
}

void Player::damage(int hp) {
	if (!is_invulnerable_) {
		n_of_lifes_ -= hp;
	}
}

void Player::heal(int hp) {
	if (n_of_lifes_ == max_n_of_lifes_)
		max_n_of_lifes_ += hp;

	n_of_lifes_ += hp;
}

void Player::addScore(int sc_to_add) {
	score_ += sc_to_add;
}

void Player::makeInvulnerable() {
	is_invulnerable_ = true;
}

void Player::throwBoomerang() {
	if (cooldown_time_ == 0.f && boomerangs_left_ != 0) {
		is_throwing_boomerang_ = true;
		cooldown_time_ = 1.f;
		boomerangs_left_--;
	}
}

void Player::checkThrowingBoomerang(float dt, CommandQueue& commands) {
	if (is_throwing_boomerang_) {
		commands.push(throw_boomerang_);
		Command update_ui;
		update_ui.action = derivedAction<UIWidget>([](UIWidget& widget, float dt) {
			; });
		update_ui.category = static_cast<unsigned int>(category::Type::InGameUI);
		is_throwing_boomerang_ = false;
	}
}

void Player::checkIsInvulnerable(float dt, CommandQueue& commands) {
	if (is_invulnerable_) {
		invulnerable_effect_time_ += dt;
		if (invulnerable_effect_time_ >= 1.f) {
			invulnerable_effect_time_ = 0.f;
			is_invulnerable_ = false;
		}
	}
}

bool Player::isThrowing() const {
	return is_throwing_boomerang_;
}

int Player::getBoomerangsLeft() const {
	return boomerangs_left_;
}

int Player::getNumberOfLifes() const {
	return n_of_lifes_;
}

int Player::getMaxNumberOfLifes() const {
	return max_n_of_lifes_;
}

int Player::getScore() const {
	return score_;
}

unsigned int Player::getCategory() const {
	return (unsigned int)category::Type::Player;
}

void Player::setKeySpeed(sf::Vector2f speed_) {
	key_speed_ = speed_;
}
