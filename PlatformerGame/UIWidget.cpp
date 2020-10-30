#include "UIWidget.h"
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>

UIWidget::UIWidget(Type type, sf::Vector2f pos) {
	type_ = type;
	sprite_.setPosition(pos);
}

Score::Score(sf::Vector2f pos, sf::Text widget_object, int score) :
	UIWidget(Type::Score, pos) {
	text_object_ = widget_object;
	score_ = score;
	text_object_.setPosition(pos);
	text_object_.setFillColor(sf::Color::White);
}

void Score::setScore(int score) {
	score_ = score;
	text_object_.setString(std::to_string(score_));
}

int Score::getScore() const {
	return score_;
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
	UIWidget(Type::Lifes, pos) {
	number_of_lifes_ = n_of_lifes;
	current_number_of_lifes_ = cur_n_of_lifes;
}

Lifes::Lifes(const TextureHolder& textures, sf::Vector2f pos, int n_of_lifes, int cur_n_of_lifes) :
	UIWidget(Type::Lifes, pos),
	void_heart_(textures.get(textures::ID::kVoidHeart)),	
	full_heart_(textures.get(textures::ID::kFullHeart)) {
	number_of_lifes_ = n_of_lifes;
	texture_.create(number_of_lifes_ * 34, 32);
	sprite_.setPosition(pos.x - 34.f * number_of_lifes_, pos.y);
	current_number_of_lifes_ = cur_n_of_lifes;
	update();
}

void Lifes::update() {
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
		sprite_.move(-34 * n_of_lifes, 0);
	}

	current_number_of_lifes_ += n_of_lifes;
	update();
}

void Lifes::deleteLife() {
	current_number_of_lifes_--;
	update();
}

void Lifes::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	rt.draw(sprite_);
}

int Lifes::getCurrentLifes() const {
	return current_number_of_lifes_;
}

Lifes& Lifes::operator+=(int lifes_to_add)
{
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
