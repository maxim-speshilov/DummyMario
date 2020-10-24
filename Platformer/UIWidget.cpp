#include "UIWidget.h"

UIWidget::UIWidget(Vector2f pos) {
	this->pos = pos; 
}

Score::Score(Vector2f pos, Text& widget_object, int score) :
UIWidget(pos) {
	this->text_object = widget_object;
	this->score = score;
	this->type = std::string("score");
	this->text_object.setPosition(pos);
	this->text_object.setFillColor(Color::White);
}

void Score::setScore(int score) {
	this->score = score;
}

int Score::getScore() const {
	return this->score;
}

void Score::draw(sf::RenderWindow& window) {
	text_object.setString(std::to_string(this->score));
	window.draw(this->text_object);
};

void Score::draw(sf::RenderTexture& rt) {
    text_object.setString(std::to_string(this->score));
	rt.draw(this->text_object);
}

void Score::addScore(int score_to_add) {
	score += score_to_add;
}
;

Score& Score::operator+= (int score_to_add) {
	addScore(score_to_add);
	return *this;
}

Lives::Lives(Vector2f pos, int n_of_lives, int cur_n_of_lives) :
UIWidget(pos) {
	numberOfLives = n_of_lives;
	currentNumberOfLives = cur_n_of_lives;
}

void Lives::addLives(int n_of_lives) {
	if (currentNumberOfLives == numberOfLives)
		numberOfLives += n_of_lives;
	currentNumberOfLives += n_of_lives;
}

void Lives::deleteLive() {
	currentNumberOfLives--;
}

Lives & Lives::operator+=(int lives_to_add)
{
	addLives(lives_to_add);
	return *this;
}

Lives & Lives::operator++() {
	addLives(1);
	return *this;
}

Lives & Lives::operator--()
{
	deleteLive();
	return *this;
}


void Lives::draw(RenderWindow& window) {
	for (int i = 0; i < numberOfLives - currentNumberOfLives; i++) {
		editor.setAnimation(Animation::AnimationType::Dead);
		editor.drawAnimation(window, pos.x - 34 * i, pos.y);
	}

	for (int i = numberOfLives - currentNumberOfLives; i < numberOfLives; i++) {
		editor.setAnimation(Animation::AnimationType::Staying);
		editor.drawAnimation(window, pos.x - 34 * i, pos.y);
	}
}


void Lives::draw(RenderTexture& rt) {
	for (int i = 0; i < numberOfLives - currentNumberOfLives; i++) {
		editor.setAnimation(Animation::AnimationType::Dead);
		editor.drawAnimation(rt, pos.x - 34 * i, pos.y);
	}

	for (int i = numberOfLives - currentNumberOfLives; i < numberOfLives; i++) {
		editor.setAnimation(Animation::AnimationType::Staying);
		editor.drawAnimation(rt, pos.x - 34 * i, pos.y);
	}
}

int Lives::getCurrentLives() const {
	return currentNumberOfLives;
}
