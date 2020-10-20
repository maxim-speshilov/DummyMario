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
	Text widget_object;
public:
	Score(Vector2f, Text&, int);
	void setScore(int);
	int getScore() const;
	void draw(sf::RenderWindow&) override;
	void draw(sf::RenderTexture&) override;
	Score& operator+= (int);
};