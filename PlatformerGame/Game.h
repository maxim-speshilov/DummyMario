#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Entity.h"
#include "UIWidget.h"
#include "World.h"
#include "InputManager.h"

class GameSettings {
public:
	float time_coef_;
	sf::Time time_per_frame_;
	std::vector <const char*> levels_file_names_;
	unsigned int level_count_;
	sf::Vector2f screen_size_;
public:
	GameSettings(sf::Time time_per_frame, float time_coef, sf::Vector2f screen_size, std::vector <const char*> levels_file_names) {
		time_per_frame_ = time_per_frame;
		time_coef_ = time_coef;
		levels_file_names_ = levels_file_names;
		screen_size_ = screen_size;
		level_count_ = 0;
	}
	~GameSettings() {
		
	}
};

class Game {
private:
	void update(float dt);
	void render();
	void processInput();

private:
	sf::Clock clock_;
	sf::RenderWindow window_;
	World world_;
	InputManager input_manager_;
	bool is_paused_;
	GameSettings settings_;
public:
	Game();
	void run();
};