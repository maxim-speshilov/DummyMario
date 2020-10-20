#include "UIWidget.h"

UIWidget::UIWidget(Vector2f pos) {
	this->pos = pos; 
}

Score::Score(Vector2f pos, Text& widget_object, int score) :
UIWidget(pos) {
	this->widget_object = widget_object;
	this->score = score;
	this->type = std::string("score");
	this->widget_object.setPosition(pos);
	this->widget_object.setFillColor(Color::White);
}

void Score::setScore(int score) {
	this->score = score;
}

int Score::getScore() const {
	return this->score;
}

void Score::draw(sf::RenderWindow& window) {
	widget_object.setString(std::to_string(this->score));
	window.draw(this->widget_object);
};

void Score::draw(sf::RenderTexture& rt) {
    widget_object.setString(std::to_string(this->score));
	rt.draw(this->widget_object);
};

Score& Score::operator+= (int score_to_add) {
	this->score += score_to_add;
	return *this;
}