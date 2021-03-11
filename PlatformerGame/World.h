#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/NonCopyable.hpp>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Scene.h"
#include "Player.h"
#include "Music.h"
#include "Sound.h"
#include "UIWidget.h"
#include "CommandQueue.h"
#include "State.h"

struct PlayerData {
	PlayerData() { ; };
	PlayerData(int score_, int lifes_, int max_n_of_lifes_, sf::Vector2f pos_, int boomerangs_left_, bool is_throwing_) :
		score(score_), lifes(lifes_), position(pos_), max_n_of_lifes(max_n_of_lifes_),
		boomerangs_left(boomerangs_left_), is_throwing(is_throwing_) {
	}

	int score;
	int lifes;
	int max_n_of_lifes;
	int boomerangs_left;
	bool is_throwing;
	sf::Vector2f position;
};

class World : private sf::NonCopyable {
public:
	explicit World(State::Context global_context);
	void update(float dt);
	void draw();

	PlayerData getPlayerData();
	CommandQueue& getCommandQueue();

private:
	void buildScene();
	void createWidgets();

	void updateView();
	void handleCollisions(float dt, CommandQueue& commands);
	void updatePlayerData();

public:
	bool isPlayerDead() const;
	bool isReachedEnd() const;

private:
	sf::RenderWindow& window_;
	sf::View view_;
	bool is_player_dead_ = false;
	bool is_reached_end_ = false;

	Scene scene_;
	Player* player_;
	PlayerData player_data_;

	TextureHolder& textures_;
	FontHolder& fonts_;
	SoundPlayer& sound_player_;
	MusicPlayer& music_player_;

	CommandQueue command_queue_;

	sf::FloatRect world_bounds_;
	sf::Vector2f spawn_position_;

	std::array<std::unique_ptr<UIWidget>, magic_enum::enum_count<UIWidget::Type>()> widgets_;

	std::list <SceneObject> scene_objects_;
	std::list <std::unique_ptr<Entity>> entities_;
};
