#include "Game.h"

struct GameSettings {
	unsigned int timeCoef;
	std::vector <const char*> levelsFileNames;
	unsigned int levelCount;
	GameSettings() {
		timeCoef = 833;
		levelsFileNames = { "levels/map1.tmx", "levels/map2.tmx" };
		levelCount = 0;
	}
};

GameSettings settings;

// TODO(me) : Create right game structure: start menu, settings, game over screen. 

Game::Game() { ; }

bool Game::run() {

	float screen_width = 480.f;
	float screen_height = 600.f;

	RenderWindow window(VideoMode(screen_width, screen_height), "Platformer", Style::Default);
	window.setMouseCursorVisible(false);
	/* ----- Add this to set max number of frames per second -----*/
	//window.setFramerateLimit(60);

	window.clear(Color::Black);


	for (auto levelFileName : settings.levelsFileNames) {
		
		Font font;
		font.loadFromFile("fonts/SuperMario256.ttf");

		Text level_name;
		level_name.setFillColor(Color::White);
		level_name.setFont(font);
		String level_name_string(string("Level ") + to_string(settings.levelCount + 1) + string(" loading..."));
		level_name.setString(level_name_string);
		unsigned int kCharacterSize = 30.f;
		level_name.setCharacterSize(kCharacterSize);
		level_name.setPosition(screen_width / 2 - 140, screen_height / 2 - 30);

		window.draw(level_name);
		window.display();

		Clock clock;
		Scene scene;

		if (!scene.loadFromXmlFile(levelFileName))
			throw logic_error("Failed to open map file");

		Music main_theme;
		if (!main_theme.openFromFile("sounds/Mario_Theme.ogg"))
			throw logic_error("Failed to open music file");

		main_theme.setVolume(80);

		Music lost_a_life;
		if (!lost_a_life.openFromFile("sounds/LostLife.ogg"))
			throw logic_error("Failed to open music file");

		lost_a_life.setVolume(70);

		SoundBuffer jump_buffer;
		if (!jump_buffer.loadFromFile("sounds/Jump.ogg"))
			throw logic_error("Failed to open sound file");

		SoundBuffer coin_buffer;
		if (!coin_buffer.loadFromFile("sounds/Coin.ogg"))
			throw logic_error("Failed to open sound file");

		Sound jump;
		jump.setBuffer(jump_buffer);
		jump.setVolume(50);

		Sound coin;
		coin.setBuffer(coin_buffer);
		coin.setVolume(65);

		float level_width = scene.getSize().x * scene.getTileSize().x;
		float level_height = scene.getSize().y * scene.getTileSize().y;


		RenderTexture game_rt;
		game_rt.create(screen_width, screen_height);
		RenderTexture game_with_ui_rt;
		game_with_ui_rt.create(screen_width, screen_height);

		View view;
		view.reset(sf::FloatRect(0.f, level_height - (float)screen_height, screen_width, screen_height));

		Texture running_set, rolling_set, jumping_set;
		entity::Player player(scene, { scene.getObjectsByType("Player").at(0).rect_.left, scene.getObjectsByType("Player").at(0).rect_.top }, 23, 28);

		running_set.loadFromFile("textures/run_set.png"); 
		rolling_set.loadFromFile("textures/rolling_set.png"), 
		jumping_set.loadFromFile("textures/jump_set.png");
		player.editor_.addAnimation(animation::kRunning, running_set, 0, 0, 23, 28, 8, 0.005, 23);
		player.editor_.addAnimation(animation::kStaying, running_set, 0, 0, 23, 28, 1, 0, 0);
		player.editor_.addAnimation(animation::kRolling, rolling_set, 0, 0, 22, 28, 8, 0.02, 22);
		player.editor_.addAnimation(animation::kJumping, jumping_set, 0, 0, 24, 28, 4, 0.005, 24);

		Texture enemy_set, moving_platform_set, moving_up_platform_set, coin_set;
		enemy_set.loadFromFile("textures/enemy_set.png");
		moving_platform_set.loadFromFile("textures/moving_platform.png");
		moving_up_platform_set.loadFromFile("textures/moving_up_platform.png");
		coin_set.loadFromFile("textures/coin.png");


		Score score(Vector2f(8, 8), Text(String("0"), font), 0);

		Texture full_heart, void_heart;
		Lives lives(Vector2f(screen_width - 34, 10.f), 3, 3);
		full_heart.loadFromFile("textures/full_heart.png");
		void_heart.loadFromFile("textures/void_heart.png");
		lives.editor_.addAnimation(animation::kStaying, full_heart, 0, 0, 32, 32, 1, 0, 0);
		lives.editor_.addAnimation(animation::kDead, void_heart, 0, 0, 32, 32, 1, 0, 0);
		
		std::list <std::unique_ptr<UIWidget>> ui_widgets;
		std::list <std::unique_ptr<UIWidget>>::iterator widgets_it;

		ui_widgets.push_front(std::make_unique <Score> (score));
		ui_widgets.push_front(std::make_unique <Lives> (lives));

		vector <Object> scene_objects = scene.getAllObjects();
			
		std::list <std::unique_ptr<entity::Entity>> entities;
		std::list <std::unique_ptr<entity::Entity>>::iterator entities_it;

		/* ----- Adding entities ----- */
		for (Object scene_object : scene.getAllObjects()) {

			if (scene_object.type_ == "Enemy") {
				entity::MoveDirection dir;
				if (scene_object.getPropertyByName("Direction") == "Right")
					dir = entity::MoveDirection::kRight;
				else
					dir = entity::MoveDirection::kLeft;
				entities.push_front(std::make_unique <entity::Enemy> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 16, 16, dir));

			} 
			if (scene_object.type_ == "Coin") {
				entities.push_front(std::make_unique <entity::Coin> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32));
			}

			if (scene_object.type_ == "MovingPlatform") {

				entities.push_front(std::make_unique <entity::MovingPlatform> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 96, 32,
					entity::MoveDirection::kRight));

			}

			if (scene_object.type_ == "MovingVerticallyPlatform") {

				entities.push_front(std::make_unique <entity::MovingPlatform> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32,
					entity::MoveDirection::kUp));

			}

			if (scene_object.type_ == "ExtraLife") {
				entities.push_front(std::make_unique <entity::ExtraLife> (scene, Vector2f(scene_object.rect_.left, scene_object.rect_.top), 32, 32));
			}
		}
	
		/* ----- Adding animations ----- */
		for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
			if ((*entities_it)->type_ == entity::Type::kEnemy) {
				(*entities_it)->editor_.addAnimation(animation::kRunning, enemy_set, 0, 25, 16, 16, 2, 0.005, 16);
				(*entities_it)->editor_.addAnimation(animation::kDead, enemy_set, 34, 33, 16, 8, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kMovingPlatform) {
				(*entities_it)->editor_.addAnimation(animation::kRunning, moving_platform_set, 0, 0, 96, 32, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kMovingVerticallyPlatform) {
				(*entities_it)->editor_.addAnimation(animation::kClimbing, moving_up_platform_set, 0, 0, 32, 32, 1, 0, 0);
			}

			if ((*entities_it)->type_ == entity::Type::kCoin) {
				(*entities_it)->editor_.addAnimation(animation::kSpinning, coin_set, 0, 0, 32, 32, 4, 0.008, 32);
			}

			if ((*entities_it)->type_ == entity::Type::kExtraLife) {
				(*entities_it)->editor_.addAnimation(animation::kStaying, full_heart, 0, 0, 32, 32, 1, 0, 0);
			}
		}

		main_theme.play();
		main_theme.setLoop(true);

		while (clock.getElapsedTime().asSeconds() < 3.f);
		clock.restart();
		bool isInvulnerable = false;
		float invulnerableCheckTime = INFINITY;
		float globalTime = 0.f;

		while (window.isOpen())
		{
			float time = clock.getElapsedTime().asSeconds();
			globalTime += time;
			clock.restart();
			time *= settings.timeCoef;

			if (main_theme.getStatus() != SoundSource::Playing && lost_a_life.getStatus() != SoundSource::Playing) {
				main_theme.play();
				lost_a_life.setPlayingOffset(seconds(0.f));
			}
			
			if ((globalTime - invulnerableCheckTime) >= 2.f) {
				invulnerableCheckTime = INFINITY;
				isInvulnerable = false;
			}

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


			player.update(time);

			if (player.rect_.intersects(scene.getFirstObject("ExitPipe").rect_)) {
				settings.levelCount++;
				window.clear(Color::Black);
				break;
			}

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
				(*entities_it)->update(time);
				if ((*entities_it)->state_ == entity::State::kDead)
					if ((*entities_it)->type_ == entity::Type::kEnemy)
						if (globalTime - (*entities_it)->death_time_ > 0.5f)
							entities_it = entities.erase(entities_it);
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
							(*entities_it)->death_time_ = globalTime;
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
								invulnerableCheckTime = globalTime;
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

							player.rect_.left += (player.speed_.x / 10 + (*entities_it)->speed_.x)*time;

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
							player.rect_.top += (*entities_it)->speed_.y*time;
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