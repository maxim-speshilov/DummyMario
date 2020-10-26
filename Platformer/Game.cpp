#include "Game.h"
#include "Sound.h"
#include "Music.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

GameSettings settings (sf::seconds(1.f / 60.f), 833.f, Vector2f(480.f, 600.f), { "levels/map1.tmx", "levels/map2.tmx" });

// TODO(me) : Create right game structure: start menu, settings, game over screen. 

Game::Game() :
	// window_(sf::VideoMode(640, 480), "Test application"),
	player_(sf::RectangleShape()) {
	player_.setSize(sf::Vector2f(50.f, 50.f));
	player_.setPosition(100.f, 100.f);
	player_.setFillColor(sf::Color::White);
}

void Game::ProcessEvents() {
	sf::Event event;
	while (window_.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			;
		}
		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Game::Update(float dt) {
		
}

void Game::Render() {
	window_.clear();
	window_.draw(player_);
	window_.display();
}

void Game::Run() {
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while (window_.isOpen()) {
		float delta_time = clock.restart().asSeconds() * settings.time_coef_;
		ProcessEvents();
		Update(delta_time);
		Render();
	}
}

bool Game::run() {

	float screen_width = 480.f;
	float screen_height = 600.f;

	RenderWindow window(VideoMode(screen_width, screen_height), "Platformer", Style::Default);
	window.setMouseCursorVisible(false);
	window.setVerticalSyncEnabled(true);

	window.clear(Color::Black);

	for (auto levelFileName : settings.levels_file_names_) {
		
		FontHolder fonts;
		fonts.load(fonts::ID::kMainFont, "fonts/SuperMario256.ttf");

		Text level_name;
		level_name.setFillColor(Color::White);
		level_name.setFont(fonts.get(fonts::ID::kMainFont));
		String level_name_string(string("Level ") + to_string(settings.level_count_ + 1) + string(" loading..."));
		level_name.setString(level_name_string);
		unsigned int kCharacterSize = 30.f;
		level_name.setCharacterSize(kCharacterSize);
		level_name.setPosition(screen_width / 2 - 140, screen_height / 2 - 30);

		window.draw(level_name);
		window.display();

		Clock clock;
		Scene scene;
		MusicPlayer music_player;
		SoundPlayer sound_player;
		
		if (!scene.loadFromXml(levelFileName))
			throw logic_error("Failed to open map file");

		Music main_theme;
		if (!main_theme.openFromFile("music/MainTheme.ogg"))
			throw logic_error("Failed to open music file");

		main_theme.setVolume(80);

		Music lost_a_life;
		if (!lost_a_life.openFromFile("sounds/LostALife.ogg"))
			throw logic_error("Failed to open music file");

		lost_a_life.setVolume(70);

		SoundBufferHolder sounds;
		sounds.load(sounds::ID::kJump, "sounds/Jump.ogg");
		sounds.load(sounds::ID::kPickedACoin, "sounds/Coin.ogg");

		Sound jump;
		jump.setBuffer(sounds.get(sounds::ID::kJump));
		jump.setVolume(50);

		Sound coin;
		coin.setBuffer(sounds.get(sounds::ID::kPickedACoin));
		coin.setVolume(65);

		float level_width = scene.getSize().x * scene.getTileSize().x;
		float level_height = scene.getSize().y * scene.getTileSize().y;

		RenderTexture game_rt;
		game_rt.create(screen_width, screen_height);
		RenderTexture game_with_ui_rt;
		game_with_ui_rt.create(screen_width, screen_height);

		View view;
		view.reset(sf::FloatRect(0.f, level_height - (float)screen_height, screen_width, screen_height));

		Score score(Vector2f(8, 8), Text(String("0"), fonts.get(fonts::ID::kMainFont)), 0);
		Lives lives(Vector2f(screen_width - 34, 10.f), 3, 3);

		TextureHolder textures;
		Player player(scene, { scene.getObjectsByType("Player").at(0).rect_.left, scene.getObjectsByType("Player").at(0).rect_.top }, 23, 28);

		textures.load(textures::ID::kRunningPlayer, "textures/run_set.png");
		textures.load(textures::ID::kRollingPlayer, "textures/rolling_set.png");
		textures.load(textures::ID::kJumpingPlayer, "textures/jump_set.png");
		textures.load(textures::ID::kEnemy, "textures/enemy_set.png");
		textures.load(textures::ID::kMovingPlatform, "textures/moving_platform.png");
		textures.load(textures::ID::kMovingVerticallyPlatform, "textures/moving_up_platform.png");
		textures.load(textures::ID::kCoin, "textures/coin.png");
		textures.load(textures::ID::kFullHeart, "textures/full_heart.png");
		textures.load(textures::ID::kVoidHeart, "textures/void_heart.png");

		std::list <std::unique_ptr<UIWidget>> ui_widgets;
		std::list <std::unique_ptr<UIWidget>>::iterator widgets_it;

		ui_widgets.push_front(std::make_unique <Score> (score));
		ui_widgets.push_front(std::make_unique <Lives> (lives));

		vector <Object> scene_objects = scene.getAllObjects();
			
		std::list <std::unique_ptr<Entity>> entities;
		std::list <std::unique_ptr<Entity>>::iterator entities_it;

		/* ----- Adding entities ----- */

		for (Object scene_object : scene.getAllObjects()) {

			if (scene_object.type_ == "Enemy") {
				entity::MoveDirection dir;
				if (scene_object.getPropertyByName("Direction") == "Right")
					dir = entity::MoveDirection::kRight;
				else
					dir = entity::MoveDirection::kLeft;
				entities.push_front(std::make_unique <Enemy> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 16, 16, dir));

			} 
			if (scene_object.type_ == "Coin") {
				entities.push_front(std::make_unique <Coin> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32));
			}

			if (scene_object.type_ == "MovingPlatform") {

				entities.push_front(std::make_unique <MovingPlatform> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 96, 32,
					entity::MoveDirection::kRight));

			}

			if (scene_object.type_ == "MovingVerticallyPlatform") {

				entities.push_front(std::make_unique <MovingPlatform> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32,
					entity::MoveDirection::kUp));

			}

			if (scene_object.type_ == "ExtraLife") {
				entities.push_front(std::make_unique <ExtraLife> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32));
			}
		}
	
		/* ----- Adding animations ----- */
		
		player.editor_.addAnimation(animation::kRunning, textures.get(textures::ID::kRunningPlayer), 0, 0, 23, 28, 8, 0.005, 23);
		player.editor_.addAnimation(animation::kStaying, textures.get(textures::ID::kRunningPlayer), 0, 0, 23, 28, 1, 0, 0);
		player.editor_.addAnimation(animation::kRolling, textures.get(textures::ID::kRollingPlayer), 0, 0, 22, 28, 8, 0.02, 22);
		player.editor_.addAnimation(animation::kJumping, textures.get(textures::ID::kJumpingPlayer), 0, 0, 24, 28, 4, 0.005, 24);

		lives.editor_.addAnimation(animation::kStaying, textures.get(textures::ID::kFullHeart), 0, 0, 32, 32, 1, 0, 0);
		lives.editor_.addAnimation(animation::kDead, textures.get(textures::ID::kVoidHeart), 0, 0, 32, 32, 1, 0, 0);

		for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
			if ((*entities_it)->type_ == entity::Type::kEnemy) {
				(*entities_it)->editor_.addAnimation(animation::kRunning, textures.get(textures::ID::kEnemy), 0, 25, 16, 16, 2, 0.005, 16);
				(*entities_it)->editor_.addAnimation(animation::kDead, textures.get(textures::ID::kEnemy), 34, 33, 16, 8, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kMovingPlatform) {
				(*entities_it)->editor_.addAnimation(animation::kRunning, textures.get(textures::ID::kMovingPlatform), 0, 0, 96, 32, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kMovingVerticallyPlatform) {
				(*entities_it)->editor_.addAnimation(animation::kClimbing, textures.get(textures::ID::kMovingVerticallyPlatform), 0, 0, 32, 32, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kCoin) {
				(*entities_it)->editor_.addAnimation(animation::kSpinning, textures.get(textures::ID::kCoin), 0, 0, 32, 32, 4, 0.008, 32);
			}

			if ((*entities_it)->type_ == entity::Type::kExtraLife) {
				(*entities_it)->editor_.addAnimation(animation::kStaying, textures.get(textures::ID::kFullHeart), 0, 0, 32, 32, 1, 0, 0);
			}
		}

		main_theme.play();
		main_theme.setLoop(true);

		while (clock.getElapsedTime().asSeconds() < 3.f) continue;
		clock.restart();

		/* ----- Updating ----- */

		bool isInvulnerable = false;
		float invulnerableCheckTime = INFINITY;
		float time_since_last_update = 0.f;
		float time = 0.f;
		float global_time = 0.f;

		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) {
					window.close();
					return false;
				}
			}
			
			// TODO(me): Think if joystick processing should be done separately and if speeed should be able to be changed by joystick movement.

			Vector2f joystick_pos = Vector2f(Joystick::getAxisPosition(0, Joystick::X), Joystick::getAxisPosition(0, Joystick::Y));

			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) || joystick_pos.x < -15.f) {
					player.is_key_pressed_["Left"] = true;
				}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) || joystick_pos.x > 15.f) {
					player.is_key_pressed_["Right"] = true;
				}
			if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) || Joystick::isButtonPressed(0, 1)) {
					player.is_key_pressed_["Up"] = true;
					if ((player.is_on_ground_) && jump.getStatus() != SoundSource::Playing)
						jump.play();
				}
			if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) || joystick_pos.y > 15.f) {
					player.is_key_pressed_["Down"] = true;
				}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					return false;
				}

			time_since_last_update += clock.restart().asSeconds() * settings.time_coef_;
			global_time += time_since_last_update; 

			if (main_theme.getStatus() != SoundSource::Playing && lost_a_life.getStatus() != SoundSource::Playing) {
				main_theme.play();
				lost_a_life.setPlayingOffset(seconds(0.f));
			}

			if ((global_time - invulnerableCheckTime) >= 2.f * settings.time_coef_) {
				invulnerableCheckTime = INFINITY;
				isInvulnerable = false;
			}

			while (time_since_last_update > settings.time_per_frame_.asSeconds() * settings.time_coef_) {
				time_since_last_update -= settings.time_per_frame_.asSeconds() * settings.time_coef_;
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed) {
						window.close();
						return false;
					}
				}

				// TODO(me): Think if joystick processing should be done separately and if speeed should be able to be changed by joystick movement.


				Vector2f joystick_pos = Vector2f(Joystick::getAxisPosition(0, Joystick::X), Joystick::getAxisPosition(0, Joystick::Y));

				if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) || joystick_pos.x < -15.f) {
					player.is_key_pressed_["Left"] = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) || joystick_pos.x > 15.f) {
					player.is_key_pressed_["Right"] = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) || Joystick::isButtonPressed(0, 1)) {
					player.is_key_pressed_["Up"] = true;
					if ((player.is_on_ground_) && jump.getStatus() != SoundSource::Playing)
						jump.play();
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) || joystick_pos.y > 15.f) {
					player.is_key_pressed_["Down"] = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					return false;
				}

				player.update(settings.time_per_frame_.asSeconds() * settings.time_coef_);

				/* ----- View processing ----- */

				Vector2f current_view_center = view.getCenter();

				if (player.rect_.left > screen_width / 2 && player.rect_.left < level_width - screen_width / 2) {
					view.setCenter(player.rect_.left, current_view_center.y);
				}

				current_view_center = view.getCenter();

				if ((player.rect_.top > view.getSize().y / 2 && player.rect_.top < scene.getSize().y * scene.getTileSize().y - view.getSize().y / 2)) {
					view.setCenter(current_view_center.x, player.rect_.top);
				}

				/* ----- Entities updating -----*/

				for (entities_it = entities.begin(); entities_it != entities.end();) {
					(*entities_it)->update(settings.time_per_frame_.asSeconds() * settings.time_coef_);
					if ((*entities_it)->state_ == entity::State::kDead)
						if ((*entities_it)->type_ == entity::Type::kEnemy)
							if (global_time - (*entities_it)->death_time_ > 0.5f * settings.time_coef_) {
								entities_it = entities.erase(entities_it);
							}
							else
								entities_it++;
						else
							entities_it = entities.erase(entities_it);
					else
						entities_it++;
				}

				/* ----- Entities processing -----*/

				for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
					if (player.rect_.intersects((*entities_it)->rect_)) {
						if ((*entities_it)->type_ == entity::Type::kEnemy) {
							if (player.speed_.y > 0) {
								(*entities_it)->speed_.x = 0;
								player.speed_.y *= (-1);
								(*entities_it)->state_ = entity::State::kDead;
								(*entities_it)->death_time_ = global_time;
							}
							else if ((*entities_it)->state_ != entity::State::kDead && !isInvulnerable) {
								if (main_theme.getStatus() == SoundSource::Playing) {
									main_theme.pause();
									lost_a_life.play();
								}
								if (lives.getCurrentLives() == 1)
									return true;
								else {
									player.state_ = entity::State::kInvulnerable;
									invulnerableCheckTime = global_time;
									isInvulnerable = true;
									lives--;
								}
							}

						}
						else if ((*entities_it)->type_ == entity::Type::kMovingPlatform) {
							if (player.speed_.y > 0) {
								player.rect_.top = (*entities_it)->rect_.top - player.rect_.height;
								player.speed_.y = 0;
								entity::MoveDirection player_dir = player.getDirection();
								entity::MoveDirection platform_dir = (*entities_it)->getDirection();

								player.rect_.left += (player.speed_.x / 10 + (*entities_it)->speed_.x) * settings.time_per_frame_.asSeconds() * settings.time_coef_;

								player.is_on_ground_ = true;
								player.state_ = entity::State::kStaying;
							}
							else if (player.speed_.y < 0) {
								player.rect_.top = (*entities_it)->rect_.top + 32;
								player.speed_.y = 0;
							}
						}

						else if ((*entities_it)->type_ == entity::Type::kMovingVerticallyPlatform) {
							if (player.speed_.y > 0) {
								player.rect_.top = (*entities_it)->rect_.top - player.rect_.height;
								player.speed_.y = 0;
								entity::MoveDirection player_dir = player.getDirection();
								entity::MoveDirection platform_dir = (*entities_it)->getDirection();
								player.rect_.top += (*entities_it)->speed_.y * time;
								player.is_on_ground_ = true;
								player.state_ = entity::State::kStaying;
							}
							else if (player.speed_.y < 0) {
								player.rect_.top = (*entities_it)->rect_.top + 32;
								player.speed_.y = 0;
							}
						}
						else if ((*entities_it)->type_ == entity::Type::kCoin) {
							coin.play();
							(*entities_it)->state_ = entity::State::kDead;
							score += 100;
						}
						else if ((*entities_it)->type_ == entity::Type::kExtraLife) {
							(*entities_it)->state_ = entity::State::kDead;
							lives++;
						}

					}
				}
			}
		
			if (player.rect_.intersects(scene.getFirstObject("ExitPipe").rect_)) {
				settings.level_count_++;
				window.clear(Color::Black);
				break;
			}

			game_rt.clear();
			game_rt.setView(view);

			scene.draw(game_rt);

			player.editor_.drawAnimation(game_rt, player.rect_.left, player.rect_.top);

			for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++)
				(*entities_it)->editor_.drawAnimation(game_rt, (*entities_it)->rect_.left, (*entities_it)->rect_.top);

			game_rt.display();

			game_with_ui_rt.clear();

			game_with_ui_rt.draw(Sprite(game_rt.getTexture()));

			score.draw(game_with_ui_rt);
			lives.draw(game_with_ui_rt);
			game_with_ui_rt.display();

			window.clear();
			window.draw(Sprite(game_with_ui_rt.getTexture()));
			window.display();
		}
	}
}

