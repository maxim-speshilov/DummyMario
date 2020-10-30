#include "UIWidget.h"

UIWidget::UIWidget(Vector2f pos) {
	this->pos_ = pos; 
}

Score::Score(Vector2f pos, Text& widget_object, int score) :
UIWidget(pos) {
	text_object_ = widget_object;
	score_ = score;
	type_ = std::string("score");
	text_object_.setPosition(pos);
	text_object_.setFillColor(Color::White);
}

void Score::setScore(int score) {
	score_ = score;
}

int Score::getScore() const {
	return score_;
}

void Score::draw(sf::RenderWindow& window) {
	text_object_.setString(std::to_string(score_));
	window.draw(text_object_);
};

void Score::draw(sf::RenderTexture& rt) {
    text_object_.setString(std::to_string(score_));
	rt.draw(this->text_object_);
}

void Score::addScore(int score_to_add) {
	score_ += score_to_add;
}
;

Score& Score::operator+= (int score_to_add) {
	addScore(score_to_add);
	return *this;
}

Lives::Lives(Vector2f pos, int n_of_lives, int cur_n_of_lives) :
UIWidget(pos) {
	number_of_lives_ = n_of_lives;
	current_number_of_lives_ = cur_n_of_lives;
}

void Lives::addLives(int n_of_lives) {
	if (current_number_of_lives_ == number_of_lives_)
		number_of_lives_ += n_of_lives;
	current_number_of_lives_ += n_of_lives;
}

void Lives::deleteLive() {
	current_number_of_lives_--;
}

Lives& Lives::operator+=(int lives_to_add)
{
	addLives(lives_to_add);
	return *this;
}

Lives& Lives::operator++() {
	addLives(1);
	return *this;
}

Lives& Lives::operator--()
{
	deleteLive();
	return *this;
}


void Lives::draw(RenderWindow& window) {
	for (int i = 0; i < number_of_lives_ - current_number_of_lives_; i++) {
		editor_.setAnimation(animation::kDead);
		editor_.drawAnimation(window, pos_.x - 34 * i, pos_.y);
	}

	for (int i = number_of_lives_ - current_number_of_lives_; i < number_of_lives_; i++) {
		editor_.setAnimation(animation::kStaying);
		editor_.drawAnimation(window, pos_.x - 34 * i, pos_.y);
	}
}


void Lives::draw(RenderTexture& rt) {
	for (int i = 0; i < number_of_lives_ - current_number_of_lives_; i++) {
		editor_.setAnimation(animation::kDead);
		editor_.drawAnimation(rt, pos_.x - 34 * i, pos_.y);
	}

	for (int i = number_of_lives_ - current_number_of_lives_; i < number_of_lives_; i++) {
		editor_.setAnimation(animation::kStaying);
		editor_.drawAnimation(rt, pos_.x - 34 * i, pos_.y);
	}
}


int Lives::getCurrentLives() const {
	return current_number_of_lives_;
}
