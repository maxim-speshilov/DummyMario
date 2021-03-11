#include "UIWidget.h"
#include "World.h"
#include <iostream>
#include <cmath>
#include <SFML/Graphics/Sprite.hpp>


UIWidget::UIWidget(Type type, sf::Vector2f pos) {
	type_ = type;
	sprite_.setPosition(pos);
}

void UIWidget::update() {

}

void UIWidget::update(PlayerData &) {

}

unsigned int UIWidget::getCategory() const {
	return (unsigned int)category::Type::InGameUI;
}

Score::Score(sf::Vector2f pos, sf::Text widget_object, int score) :
	UIWidget(Type::kScore, pos) {
	text_object_ = widget_object;
	score_ = score;
	text_object_.setPosition(pos);
	text_object_.setFillColor(sf::Color::White);
	text_object_.setOutlineColor(sf::Color::Black);
	text_object_.setOutlineThickness(2.f);
	text_object_.setScale(2.f, 2.f);
}

void Score::setScore(int score) {
	score_ = score;
	text_object_.setString(std::to_string(score_));
}

int Score::getScore() const {
	return score_;
}

void Score::update(float dt) {

}

void Score::update(PlayerData& data) {
	score_ = data.score;
	text_object_.setString(std::to_string(score_));
}


void Score::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	rt.draw(text_object_, states);
}

void Score::addScore(int score_to_add) {
	score_ += score_to_add;
	text_object_.setString(std::to_string(score_));
}

Score& Score::operator+= (int score_to_add) {
	addScore(score_to_add);
	return *this;
}

Lifes::Lifes(sf::Vector2f pos, int n_of_lifes, int cur_n_of_lifes) :
	UIWidget(Type::kLifes, pos) {
	number_of_lifes_ = n_of_lifes;
	current_number_of_lifes_ = cur_n_of_lifes;
}

Lifes::Lifes(const TextureHolder& textures, sf::Vector2f pos, int n_of_lifes, int cur_n_of_lifes) :
	UIWidget(Type::kLifes, pos),
	void_heart_(textures.get(Textures::kVoidHeart)),	
	full_heart_(textures.get(Textures::kFullHeart)) {
	number_of_lifes_ = n_of_lifes;
	texture_.create(number_of_lifes_ * 34, 32);
	texture_.update(full_heart_, 32, 0);
	sprite_.setPosition(pos.x - 34.f * 2.f * number_of_lifes_, pos.y);
	sprite_.setTexture(texture_);
	sprite_.setScale(2.f, 2.f);
	current_number_of_lifes_ = cur_n_of_lifes;
	update(0.f);
}

void Lifes::update(float dt) {
	texture_.create(number_of_lifes_ * 34, 32);
	
	for (int i = 0; i < current_number_of_lifes_; i++) 
		texture_.update(full_heart_, 34 * i, 0);

	for (int i = current_number_of_lifes_; i < number_of_lifes_; i++)
		texture_.update(void_heart_, 34 * i, 0);

	sprite_.setTexture(texture_, true);
}

void Lifes::update(PlayerData& data) {
	if (data.lifes > number_of_lifes_)
		sprite_.move(-34 * 2.f, 0);

	number_of_lifes_ = data.max_n_of_lifes;
	current_number_of_lifes_ = data.lifes;

	texture_.create(number_of_lifes_ * 34, 32);

	for (int i = 0; i < current_number_of_lifes_; i++)
		texture_.update(full_heart_, 34 * i, 0);

	for (int i = current_number_of_lifes_; i < number_of_lifes_; i++)
		texture_.update(void_heart_, 34 * i, 0);

	sprite_.setTexture(texture_, true);
}


void Lifes::addLifes(int n_of_lifes) {
	if (current_number_of_lifes_ == number_of_lifes_) {
		number_of_lifes_ += n_of_lifes;
		sprite_.move(-34 * 2.f * n_of_lifes, 0);
	}

	current_number_of_lifes_ += n_of_lifes;
	update(0.f);
}

void Lifes::deleteLife() {
	current_number_of_lifes_--;
	update(0.f);
}

void Lifes::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	rt.draw(sprite_);
}

void Lifes::setCurrentLifes(int n_of_lifes) {
	current_number_of_lifes_ = n_of_lifes;
}

int Lifes::getCurrentLifes() const {
	return current_number_of_lifes_;
}

Lifes& Lifes::operator+=(int lifes_to_add) {
	addLifes(lifes_to_add);
	return *this;
}

Lifes& Lifes::operator++(int) {
	addLifes(1);
	return *this;
}

Lifes& Lifes::operator--(int) {
	deleteLife();
	return *this;
}

Boomerangs::Boomerangs(const TextureHolder& textures, const FontHolder& fonts, sf::Vector2f pos) :
	UIWidget(Type::kBoomerangs, pos) {
	sf::Vector2f scale(4.f, 4.f);

	utility::centerOrigin(sprite_);
	sprite_.setPosition(pos);
	sprite_.setTexture(textures.get(Textures::kBoomerang));
	sprite_.setTextureRect(sf::IntRect(0, 0, 16, 16));
	sprite_.setScale(scale);

	utility::centerOrigin(n_of_left_);
	n_of_left_.setFont(fonts.get(Fonts::kMain));
	n_of_left_.setPosition(pos + scale * sf::Vector2f(20.f, -0.5f));
	n_of_left_.setFillColor(sf::Color::White);
	n_of_left_.setOutlineColor(sf::Color::Black);
	n_of_left_.setString("10");
	n_of_left_.setOutlineThickness(2.f);
	n_of_left_.setScale(sf::Vector2f(64.f / n_of_left_.getCharacterSize(), 64.f / n_of_left_.getCharacterSize()));

	size_t n = 62;
	auto angle_step = 360.f / (n - 2);
	float angle = 90.f;
	auto color = sf::Color(0, 0, 0, 150);
	float r = sqrt(32.f * 32.f + 32.f * 32.f) - 1.f;

	countdown_circle_.push_back(sf::Vertex(pos + sf::Vector2f(32.f, 32.f), color));

}

void Boomerangs::draw(sf::RenderTarget & rt, sf::RenderStates states) const {
	rt.draw(sprite_, states);
	rt.draw(n_of_left_, states);
	//auto bounds = sprite_.getGlobalBounds();
	//auto r = sqrt(pow(bounds.width, 2) + pow(bounds.height, 2)) / 2.f;
	//sf::CircleShape circle(40.f);
	//circle.setOrigin(sf::Vector2f(38.f, 38.f));
	//circle.setPosition(sprite_.getPosition() + sf::Vector2f(32.f, 32.f));
	//circle.setFillColor(sf::Color::Transparent);
	//circle.setOutlineColor(sf::Color::Black);
	//circle.setOutlineThickness(6.f);
	//rt.draw(circle);
	if (countdown_circle_.size() != 1)
		rt.draw(&countdown_circle_[0], countdown_circle_.size(), sf::TriangleFan, states);
}

void Boomerangs::update(PlayerData& player_data) {
	if (countdown_circle_.size() != 1)
		countdown_circle_.pop_back();

	n_of_left_.setString(std::to_string(player_data.boomerangs_left));
}

void Boomerangs::startCounter() {
	size_t n = 62;
	auto angle_step = 360.f / (n - 2);
	float angle = 90.f;
	auto color = sf::Color(0, 0, 0, 150);
	float r = sqrt(32.f * 32.f + 32.f * 32.f) - 1.f;
	auto pos = countdown_circle_[0].position;

	for (size_t i = 1; i < n; ++i) {
		auto angle_ = utility::radian(-90.f - angle_step * (i - 1));
		countdown_circle_.push_back(sf::Vertex(r * sf::Vector2f(cos(angle_), sin(angle_)) + pos + sf::Vector2f(32.f, 32.f), color));
	}
}

