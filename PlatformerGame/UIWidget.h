#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "AnimationPlayer.h"
#include "SceneObject.h"
#include <list>
#include <string>

class UIWidget : public SceneObject {
public:
	enum class Type {
		Lifes,
		Score
	};

protected:
	sf::Sprite sprite_;
	Type type_;

public:
	UIWidget(Type type, sf::Vector2f);
};

class Score : public UIWidget {
public:
	Score(sf::Vector2f, sf::Text, int);

	void addScore(int);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	void setScore(int);
	int getScore() const;

	Score& operator+= (int);

private:
	int score_;
	sf::Text text_object_;
};

class Lifes : public UIWidget {
public:
	Lifes(sf::Vector2f, int, int);
	Lifes(const TextureHolder&, sf::Vector2f, int, int);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	void update();

	void addLifes(int);
	void deleteLife();

	int getCurrentLifes() const;

	Lifes& operator+= (int);
	Lifes& operator++ (int);
	Lifes& operator-- (int);

private:
	sf::Texture void_heart_;
	sf::Texture full_heart_;
	sf::Texture texture_;
	unsigned int number_of_lifes_;
	unsigned int current_number_of_lifes_;
};