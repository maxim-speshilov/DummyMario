#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include <string>

using namespace sf;

class UIWidget {
protected:
	Vector2f pos;
	AnimationEditor editor;
	std::string type;
public:
	UIWidget(Vector2f);
	friend bool gameLoop();
	virtual void draw(sf::RenderWindow&) = 0;
	virtual void draw(sf::RenderTexture&) = 0;
};

class Score : public UIWidget {
private:
	int score;
	Text text_object;
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
	unsigned int numberOfLives;
	unsigned int currentNumberOfLives;
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