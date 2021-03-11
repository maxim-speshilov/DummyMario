#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "AnimationPlayer.h"
#include "SceneObject.h"
#include "Player.h"
#include <list>
#include <string>

struct PlayerData;

class UIWidget : public SceneObject {
public:
	enum class Type {
		kLifes,
		kScore,
		kBoomerangs
	};

protected:
	sf::Sprite sprite_;
	Type type_;

public:
	UIWidget(Type type, sf::Vector2f);

	virtual void update();
	virtual void update(PlayerData&);

	unsigned int getCategory() const override;
};

class Score : public UIWidget {
public:
	Score(sf::Vector2f, sf::Text, int);

	void addScore(int);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	void update(float dt);
	void update(PlayerData&) override;

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

	void update(float dt);
	void update(PlayerData&) override;

	void addLifes(int);
	void deleteLife();

	void setCurrentLifes(int n_of_lifes);
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

class Boomerangs : public UIWidget {
public:
	Boomerangs(const TextureHolder&, const FontHolder&, sf::Vector2f);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	void update(PlayerData&) override;

	void startCounter();

private:
	sf::Text n_of_left_;
	std::vector<sf::Vertex> countdown_circle_;
};