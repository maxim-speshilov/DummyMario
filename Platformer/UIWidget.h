#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include <string>

using namespace sf;

class UIWidget {
public:
	Vector2f pos_;
	animation::AnimationEditor editor_;
	std::string type_;
public:
	UIWidget(Vector2f);
	virtual void draw(sf::RenderWindow&) = 0;
	virtual void draw(sf::RenderTexture&) = 0;
};

class Score : public UIWidget {
private:
	int score_;
	Text text_object_;
public:
	Score(Vector2f, Text&, int);
	void setScore(int);
	int getScore() const;
	void draw(sf::RenderWindow&) override;
	void draw(sf::RenderTexture&) override;
	void addScore(int);
	Score& operator+= (int);
};

class Lives : public UIWidget {
private:
	unsigned int number_of_lives_;
	unsigned int current_number_of_lives_;
public:
	Lives(Vector2f, int, int);
	void draw(sf::RenderWindow&) override;
	void draw(sf::RenderTexture&) override;
	int getCurrentLives() const;
	void addLives(int);
	void deleteLive();
	Lives& operator+= (int);
	Lives& operator++ ();
	Lives& operator-- ();
};