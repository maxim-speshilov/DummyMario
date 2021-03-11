//#include "Game.h"
//#include "Player.h"
//#include "Enemy.h"
//#include "Entity.h"
//#include "MovingPlatform.h"
//#include "Pickup.h"
//#include "Sound.h"
//#include "Music.h"
//#include "ResourceHolder.h"
//#include "ResourceIdentifiers.h"
//#include "InputManager.h"
//#include <iostream>
//
//
//GameSettings settings(sf::seconds(1.f / 60.f), 833.f, 
//	sf::Vector2f(480.f, 600.f), { "levels/map1.tmx", "levels/map2.tmx" });
//
//Game::Game() :
//	window_(sf::VideoMode(1920, 1080), "Test", sf::Style::Fullscreen),
//	world_(window_),
//	input_manager_(),
//	is_paused_(false),
//	settings_(sf::seconds(1.f / 60.f), 833.f,
//		sf::Vector2f(1920.f, 1080.f), { "levels/map1.tmx", "levels/map2.tmx" }) {
//
//}
//
//void Game::update(float dt) {
//	world_.update(dt);
//}
//
//void Game::render() {
//	window_.clear();
//	world_.draw();
//	window_.display();
//}
//
//void Game::processInput() {
//	CommandQueue& commands = world_.getCommandQueue();
//
//	sf::Event event;
//	while (window_.pollEvent(event)) {
//		input_manager_.handleEvent(event, commands);
//		switch (event.type) {
//		case sf::Event::Closed:
//			window_.close();
//			break;
//		case sf::Event::GainedFocus:
//			is_paused_ = false;
//			break;
//		case sf::Event::LostFocus:
//			is_paused_ = true;
//			break;
//		default:
//			break;
//		}
//	}
//	
//	input_manager_.handleRealtimeInput(commands);
//}
//
//void Game::run() {
//	sf::Clock clock;
//	float time_since_last_update = 0.f;
//	while (window_.isOpen()) {
//		float dt = clock.restart().asSeconds();
//		time_since_last_update += dt;
//		while (time_since_last_update > (1.f / 60.f)) {
//			time_since_last_update -= (1.f / 60.f);
//			processInput();
//			update(1.f / 60.f);
//		}
//		render();
//	}
//}
//
//using namespace sf;
//
//bool Game::run() {
//	float screen_width = 480.f;
//	float screen_height = 600.f;
//
//	RenderWindow window(VideoMode(screen_width, screen_height), "Platformer", 
//		Style::Default);
//
//	window.display();
//	window.setMouseCursorVisible(false);
//	window.setFramerateLimit(60);
//	window.setVerticalSyncEnabled(true);
//
//	window.clear(Color::Black);
//
//	for (auto levelFileName : settings.levels_file_names_) {
//		FontHolder fonts;
//		fonts.load(Fonts::kMain, "fonts/SuperMario256.ttf");
//
//		Text level_name;
//		level_name.setFillColor(Color::White);
//		level_name.setFont(fonts.get(Fonts::kMain));
//		String level_name_string(std::string("Level ") 
//			+ std::to_string(settings.level_count_ + 1) 
//			+ std::string(" loading..."));
//		level_name.setString(level_name_string);
//		unsigned int kCharacterSize = 30.f;
//		level_name.setCharacterSize(kCharacterSize);
//		level_name.setPosition(screen_width / 2 - 140, 
//			screen_height / 2 - 30);
//
//		window.draw(level_name);
//		window.display();
//
//		Clock clock;
//		Scene scene;
//		MusicPlayer music_player;
//		SoundPlayer sound_player;
//
//		if (!scene.loadFromXml(levelFileName))
//			throw std::logic_error("Failed to open map file");
//
//		SoundBufferHolder sounds;
//		sounds.load(Sounds::kJump, "sounds/Jump.ogg");
//		sounds.load(Sounds::kPickedACoin, "sounds/Coin.ogg");
//
//		Sound jump;
//		jump.setBuffer(sounds.get(Sounds::kJump));
//		jump.setVolume(50);
//
//		Sound coin;
//		coin.setBuffer(sounds.get(Sounds::kPickedACoin));
//		coin.setVolume(65);
//
//		float level_width = scene.getSize().x * scene.getTileSize().x;
//		float level_height = scene.getSize().y * scene.getTileSize().y;
//
//		RenderTexture game_rt;
//		game_rt.create(screen_width, screen_height);
//		RenderTexture game_with_ui_rt;
//		game_with_ui_rt.create(screen_width, screen_height);
//
//		View view;
//		view.reset(sf::FloatRect(0.f, level_height - (float)screen_height, screen_width, screen_height));
//
//		TextureHolder textures;
//
//		textures.load(Textures::kRunningPlayer, "textures/run_set.png");
//		textures.load(Textures::kRollingPlayer, "textures/rolling_set.png");
//		textures.load(Textures::kJumpingPlayer, "textures/jump_set.png");
//		textures.load(Textures::kEnemy, "textures/enemy_set.png");
//		textures.load(Textures::kMovingPlatform, "textures/moving_platform.png");
//		textures.load(Textures::kMovingVerticallyPlatform, "textures/moving_up_platform.png");
//		textures.load(Textures::kCoin, "textures/coin.png");
//		textures.load(Textures::kFullHeart, "textures/full_heart.png");
//		textures.load(Textures::kVoidHeart, "textures/void_heart.png");
//
//		Score score = Score(Vector2f(8, 8), Text(String("0"), fonts.get(Fonts::kMain)), 0);
//		Lifes lifes = Lifes(textures, Vector2f(screen_width - 10.f, 10.f), 3, 3);
//
//		Player player(scene, textures, scene.getObjectsByType(SceneObject::Type::kPlayer).front().rect_.getPosition(), 23, 28);
//
//		std::list <std::unique_ptr<UIWidget>> ui_widgets;
//		std::list <std::unique_ptr<UIWidget>>::iterator widgets_it;
//
//		ui_widgets.push_front(std::make_unique <Score>(score));
//		ui_widgets.push_front(std::make_unique <Lifes>(lifes));
//
//		std::list <SceneObject> scene_objects = scene.getObjectsExcept(Scene::ObjectGroup::Type::MapObjects);
//		std::list <std::unique_ptr<Entity>> entities;
//		std::list <std::unique_ptr<Entity>>::iterator entities_it;
//
//		CommandQueue commands;
//
//		/* ----- Adding entities ----- */
//
//		for (SceneObject& scene_object : scene_objects) {
//			if (scene_object.type_ == SceneObject::Type::kEnemy) {
//				entities.push_front(std::make_unique<Enemy>(scene, textures, scene_object.rect_.getPosition(), 16, 16,
//					entities::MoveDirection::kRight));
//			}
//
//			if (scene_object.type_ == SceneObject::Type::kCoin) {
//				entities.push_front(std::make_unique<Pickup>(scene, entities::Type::kCoin, textures, scene_object.rect_.getPosition(), 32, 32));
//			}
//
//			if (scene_object.type_ == SceneObject::Type::kMovingPlatform) {
//				entities.push_front(std::make_unique<MovingPlatform>(scene, textures, scene_object.rect_.getPosition(), 96, 32,
//					entities::MoveDirection::kRight));
//			}
//
//			if (scene_object.type_ == SceneObject::Type::kMovingVerticallyPlatform) {
//				entities.push_front(std::make_unique<MovingPlatform>(scene, textures, scene_object.rect_.getPosition(), 32, 32,
//					entities::MoveDirection::kUp));
//			}
//
//			if (scene_object.type_ == SceneObject::Type::kExtraLife) {
//				entities.push_front(std::make_unique<Pickup>(scene, entities::Type::kExtraLife, textures, scene_object.rect_.getPosition(), 32, 32));
//			}
//		}
//
//		music_player.play(Music::kMainTheme);
//		music_player.setLoop(true);
//
//		while (clock.getElapsedTime().asSeconds() < 3.f) continue;
//		clock.restart();
//
//		/* ----- Updating ----- */
//
//		bool isInvulnerable = false;
//		float invulnerableCheckTime = INFINITY;
//		float time_since_last_update = 0.f;
//		float time = 0.f;
//		float global_time = 0.f;
//
//		while (window.isOpen())
//		{
//			Event event;
//			while (window.pollEvent(event))
//			{
//				if (event.type == Event::Closed) {
//					window.close();
//					return false;
//				}
//			}
//
//			time_since_last_update += clock.restart().asSeconds();
//			global_time += time_since_last_update;
//
//			if (music_player.getStatus(Music::kMainTheme) != SoundSource::Playing &&
//				music_player.getStatus(Music::kLostALife) != SoundSource::Playing) {
//				music_player.setLoop(true);
//				music_player.play(Music::kMainTheme);
//			}
//
//			if ((global_time - invulnerableCheckTime) >= 2.f) {
//				invulnerableCheckTime = INFINITY;
//				isInvulnerable = false;
//			}
//
//			while (time_since_last_update > settings.time_per_frame_.asSeconds()) {
//				time_since_last_update -= settings.time_per_frame_.asSeconds();
//				Event event;
//				while (window.pollEvent(event)) {
//					if (event.type == Event::Closed) {
//						window.close();
//						return false;
//					}
//					input_manager_.handleEvent(event, commands);
//				}
//
//				/* Input processing */
//
//				input_manager_.handleRealtimeInput(commands);
//
//				while (!commands.isEmpty()) 
//					player.onCommand(commands.pop(), settings.time_per_frame_.asSeconds());
//
//				player.update(settings.time_per_frame_.asSeconds());
//
//				/* ----- View processing ----- */
//
//				Vector2f current_view_center = view.getCenter();
//
//				if (player.rect_.getPosition().x > screen_width / 2 && player.rect_.getPosition().x < level_width - screen_width / 2) {
//					view.setCenter(player.rect_.getPosition().x, current_view_center.y);
//				}
//
//				current_view_center = view.getCenter();
//
//				if ((player.rect_.getPosition().y > view.getSize().y / 2 && player.rect_.getPosition().y < scene.getSize().y * scene.getTileSize().y - view.getSize().y / 2)) {
//					view.setCenter(current_view_center.x, player.rect_.getPosition().y);
//				}
//
//				/* ----- Entities updating -----*/
//
//				for (entities_it = entities.begin(); entities_it != entities.end();) {
//					(*entities_it)->update(settings.time_per_frame_.asSeconds());
//					if ((*entities_it)->state_ == entities::State::kDead)
//						if ((*entities_it)->type_ == entities::Type::kEnemy)
//							if (global_time - (*entities_it)->death_time_ > 0.5f) {
//								entities_it = entities.erase(entities_it);
//							}
//							else
//								entities_it++;
//						else
//							entities_it = entities.erase(entities_it);
//					else
//						entities_it++;
//				}
//
//				/* ----- Entities processing -----*/
//
//				for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
//					if (player.rect_.getGlobalBounds().intersects((*entities_it)->rect_.getGlobalBounds())) {
//						if ((*entities_it)->type_ == entities::Type::kEnemy) {
//							if (player.speed_.y > 0) {
//								(*entities_it)->speed_.x = 0;
//								player.speed_.y *= (-1);
//								(*entities_it)->state_ = entities::State::kDead;
//								(*entities_it)->death_time_ = global_time;
//							}
//							else if ((*entities_it)->state_ != entities::State::kDead && !isInvulnerable) {
//								music_player.stop();
//								music_player.setLoop(false);
//								music_player.play(Music::kLostALife);
//								if (lifes.getCurrentLifes() == 1)
//									return true;
//								else {
//									player.state_ = entities::State::kInvulnerable;
//									invulnerableCheckTime = global_time;
//									isInvulnerable = true;
//									lifes--;
//								}
//							}
//						}
//						else if ((*entities_it)->type_ == entities::Type::kMovingPlatform) {
//							if (player.speed_.y > 0) {
//								setPositionY(player.rect_, (*entities_it)->rect_.getPosition().y - player.rect_.getSize().y);
//								player.speed_.y = 0;
//
//								entities::MoveDirection player_dir = player.getDirection();
//								entities::MoveDirection platform_dir = (*entities_it)->getDirection();
//
//								player.rect_.move(sf::Vector2f((player.speed_.x / 10 +
//									(*entities_it)->speed_.x) * settings.time_per_frame_.asSeconds(), 0.f));
//
//								player.is_on_ground_ = true;
//								player.state_ = entities::State::kStaying;
//							}
//							else if (player.speed_.y < 0) {
//								setPositionY(player.rect_, (*entities_it)->rect_.getPosition().y + 32);
//								player.speed_.y = 0;
//							}
//						}
//
//						else if ((*entities_it)->type_ == entities::Type::kMovingVerticallyPlatform) {
//							if (player.speed_.y > 0) {
//								setPositionY(player.rect_, (*entities_it)->rect_.getPosition().y - player.rect_.getSize().y);
//								player.speed_.y = 0;
//								entities::MoveDirection player_dir = player.getDirection();
//								entities::MoveDirection platform_dir = (*entities_it)->getDirection();
//								player.rect_.move(0.f, (*entities_it)->speed_.y * time);
//								player.is_on_ground_ = true;
//								player.state_ = entities::State::kStaying;
//							}
//							else if (player.speed_.y < 0) {
//								setPositionY(player.rect_, (*entities_it)->rect_.getPosition().y + 32);
//								player.speed_.y = 0;
//							}
//						}
//						else if ((*entities_it)->type_ == entities::Type::kCoin) {
//							coin.play();
//							(*entities_it)->state_ = entities::State::kDead;
//							(score) += 100;
//						}
//						else if ((*entities_it)->type_ == entities::Type::kExtraLife) {
//							(*entities_it)->state_ = entities::State::kDead;
//							lifes++;
//						}
//					}
//				}
//			}
//
//			if (player.rect_.getGlobalBounds().intersects(scene.getFirstObject(SceneObject::Type::kExitPipe).rect_.getGlobalBounds())) {
//				settings.level_count_++;
//				window.clear(Color::Black);
//				break;
//			}
//
//			game_rt.clear();
//			game_rt.setView(view);
//
//			game_rt.draw(scene);
//
//			game_rt.draw(player);
//
//			for (auto& entity : entities)
//				game_rt.draw(*entity);
//
//			game_rt.display();
//
//			game_with_ui_rt.clear();
//
//			game_with_ui_rt.draw(Sprite(game_rt.getTexture()));
//
//			game_with_ui_rt.draw(score);
//			game_with_ui_rt.draw(lifes);
//			game_with_ui_rt.display();
//
//			window.clear();
//			window.draw(Sprite(game_with_ui_rt.getTexture()));
//			window.display();
//		}
//	}
//}