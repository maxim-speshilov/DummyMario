#include "World.h"
#include "Utility.h"
#include "Enemy.h"
#include "Boomerang.h"
#include "MovingPlatform.h"
#include "Pickup.h"
#include <iostream>
#include <chrono>

World::World(State::Context context) :
	window_(*context.window),
    view_(context.window->getDefaultView()),
	textures_(*context.textures),
	fonts_(*context.fonts),
	scene_(),
	spawn_position_(128, 1056),
	music_player_(*context.music_player),
	sound_player_(*context.sound_player),
	player_data_() {

	buildScene();
	createWidgets();
	world_bounds_ = sf::FloatRect(sf::Vector2f(0.f, 0.f), scene_.getSize() * scene_.getTileSize());

	float kViewHeight = 400.f;
	float kViewWidth = kViewHeight * (1920.f / 1080.f);
	view_.reset(sf::FloatRect(0.f, world_bounds_.height - kViewHeight, kViewWidth, kViewHeight));
	updateView();

}

void World::update(float dt) {
	if (music_player_.getStatus(Tracks::kMainTheme) != sf::SoundSource::Playing &&
		music_player_.getStatus(Tracks::kLostALife) != sf::SoundSource::Playing) {
		music_player_.setLoop(true);
		music_player_.play(Tracks::kMainTheme);
	}

	while (!command_queue_.isEmpty())
		scene_.onCommand(command_queue_.pop(), dt);

	scene_.update(dt, command_queue_);

	handleCollisions(dt, command_queue_);
	scene_.removeRemains();

	updatePlayerData();

	for (auto& widget : widgets_)
		widget->update(player_data_);
}

void World::draw() {
	updateView();
	window_.setView(view_);
	window_.draw(scene_);

	for (auto& entity : entities_)
		window_.draw(*entity);

	window_.setView(window_.getDefaultView());

	for (auto& widget : widgets_)
		window_.draw(*widget);
}

PlayerData World::getPlayerData() {
	return PlayerData(player_->getScore(), player_->getNumberOfLifes(), 
		player_->getMaxNumberOfLifes(), player_->getPosition(), player_->getBoomerangsLeft(), player_->isThrowing());
}

CommandQueue& World::getCommandQueue() {
	return command_queue_;
}

void World::buildScene() {
	if (!scene_.loadFromXml("levels/map1.tmx"))
		throw std::logic_error("Failed to open map file");

	textures_.load(Textures::kRunningPlayer, "textures/run_set.png");
	textures_.load(Textures::kRollingPlayer, "textures/rolling_set.png");
	textures_.load(Textures::kJumpingPlayer, "textures/jump_set.png");
	textures_.load(Textures::kBoomerang, "textures/boomerang.png");

	auto player = std::make_unique<Player>(SceneObject::Type::kPlayer, textures_);
	player->setPosition(126.f, 1057.f);
	player_ = player.get();
	
	scene_.addObjectToGroup(Scene::ObjectGroup::Player, std::move(player));
}

void World::createWidgets() {
	std::unique_ptr<Score> score = std::make_unique<Score>(sf::Vector2f(8.f, 8.f), sf::Text(sf::String("0"), fonts_.get(Fonts::kMain)), 0);
	std::unique_ptr<Lifes> lifes = std::make_unique<Lifes>(textures_, sf::Vector2f(window_.getSize().x - 10.f, 10.f), 3, 3);
	std::unique_ptr<Boomerangs> boomer = std::make_unique<Boomerangs>(textures_, fonts_, sf::Vector2f(40.f, window_.getSize().y - 74.f));
	widgets_[(int)UIWidget::Type::kScore] = std::move(score);
	widgets_[(int)UIWidget::Type::kLifes] = std::move(lifes);
	widgets_[(int)UIWidget::Type::kBoomerangs] = std::move(boomer);
}

void World::updateView() {
	sf::Vector2f current_view_center = view_.getCenter();

	if (player_->getPosition().x > view_.getSize().x / 2.f
		&& player_->getPosition().x < world_bounds_.width - view_.getSize().x / 2.f) 
		view_.setCenter(player_->getPosition().x, current_view_center.y);
	
	current_view_center = view_.getCenter();

	if (player_->getPosition().y > view_.getSize().y / 2.f
		&& player_->getPosition().y < world_bounds_.height - view_.getSize().y / 2.f)
		view_.setCenter(current_view_center.x, player_->getPosition().y);
}

bool matchesCategories(std::pair<SceneObject*, SceneObject*>& colliders, SceneObject::Type type1, SceneObject::Type type2)
{
	SceneObject::Type category1 = colliders.first->getType();
	SceneObject::Type category2 = colliders.second->getType();

	if (type1 == category1 && type2 == category2) {
		return true;
	}
	else if (type1 == category2 && type2 == category1) {
		std::swap(colliders.first, colliders.second);
		return true;
	} else {
		return false;
	}
}

void World::handleCollisions(float dt, CommandQueue& commands) {
	std::set<std::pair<SceneObject*, SceneObject*>> pairs;

	scene_.checkObjectCollisions(pairs);

	for (auto pair : pairs) {
		if (matchesCategories(pair, SceneObject::Type::kEnemy, SceneObject::Type::kSolid) || 
			matchesCategories(pair, SceneObject::Type::kEnemy, SceneObject::Type::kEnemyBorder)) {
			auto& enemy = static_cast<Enemy&>(*pair.first);
			auto& object = *pair.second;
			if (enemy.getDirection() == entities::MoveDirection::kRight) {
				enemy.setPositionX(object.getPosition().x - enemy.getSize().x);
				enemy.setDirection(entities::MoveDirection::kLeft);
				enemy.setSpeedX(-enemy.getSpeed().x);
			}
			else {
				enemy.setPositionX(object.getPosition().x + object.getSize().x);
				enemy.setDirection(entities::MoveDirection::kRight);
				enemy.setSpeedX(-enemy.getSpeed().x);
			}

		}

		else if (matchesCategories(pair, SceneObject::Type::kBoomerang, SceneObject::Type::kSolid) || 
			matchesCategories(pair, SceneObject::Type::kBoomerang, SceneObject::Type::kMovingPlatform) ||
			matchesCategories(pair, SceneObject::Type::kBoomerang, SceneObject::Type::kMovingVerticallyPlatform)) {
			auto& boomerang = static_cast<Boomerang&>(*pair.first);
			boomerang.damage(1.f);
		}

		else if (matchesCategories(pair, SceneObject::Type::kBoomerang, SceneObject::Type::kEnemy)) {
			auto& boomerang = static_cast<Boomerang&>(*pair.first);
			auto& enemy = static_cast<Enemy&>(*pair.second);

			boomerang.damage(1.f);
			enemy.damage(1.f);
		}

		else if (matchesCategories(pair, SceneObject::Type::kMovingPlatform, SceneObject::Type::kSolid) ||
			matchesCategories(pair, SceneObject::Type::kMovingPlatform, SceneObject::Type::kPlatformBorder)) {
			auto& platform = static_cast<MovingPlatform&>(*pair.first);
			auto& object = *pair.second;
			switch (platform.getDirection()) {
			case entities::MoveDirection::kRight:
				platform.setPositionX(object.getPosition().x - platform.getSize().x);
				platform.setDirection(entities::MoveDirection::kLeft);
				platform.setSpeedX(-platform.getSpeed().x);
				break;
			case entities::MoveDirection::kLeft:
				platform.setPositionX(object.getPosition().x + object.getSize().x);
				platform.setDirection(entities::MoveDirection::kRight);
				platform.setSpeedX(-platform.getSpeed().x);
				break;
			default:
				break;
			}
		}

		else if (matchesCategories(pair, SceneObject::Type::kMovingVerticallyPlatform, SceneObject::Type::kSolid) ||
			matchesCategories(pair, SceneObject::Type::kMovingVerticallyPlatform, SceneObject::Type::kPlatformBorder)) {
			auto& platform = static_cast<MovingPlatform&>(*pair.first);
			auto& object = *pair.second;
			switch (platform.getDirection()) {
			case entities::MoveDirection::kUp:
				platform.setPositionY(object.getPosition().y + object.getSize().y);
				platform.setDirection(entities::MoveDirection::kDown);
				platform.setSpeedY(-platform.getSpeed().y);
				break;
			case entities::MoveDirection::kDown:
				platform.setPositionY(object.getPosition().y - platform.getSize().y);
				platform.setDirection(entities::MoveDirection::kUp);
				platform.setSpeedY(-platform.getSpeed().y);
				break;
			default:
				break;
			}
		}

		else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kSolid)) {
			auto& player = static_cast<Player&>(*pair.first);
			auto& object = *pair.second;

			if (player_data_.position.y + player.getSize().y <= object.getPosition().y) {
				player.setPositionY(object.getPosition().y - player.getSize().y);
				player.setSpeedY(0);
				player.is_on_ground_ = true;
				player.setState(entities::State::kStaying);
			}

			else if (player_data_.position.y >= object.getPosition().y + object.getSize().y) {
				player.setPositionY(object.getPosition().y + object.getSize().y);
				player.setSpeedY(0);
			}

			else if (player_data_.position.x >= object.getPosition().x + object.getSize().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x + object.getSize().x);
			} 

			else if (player_data_.position.x + player.getSize().x <= object.getPosition().x + object.getSize().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x - player.getSize().x);
			}
		}

		else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kJumpBooster) ||
			matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kJumpSuperBooster)) {

			auto& player = static_cast<Player&>(*pair.first);
			auto& object = *pair.second;

			if (player.getPosition().y + player.getSize().y > object.getPosition().y &&
				player_data_.position.y + player.getSize().y <= object.getPosition().y) {
				player.setPositionY(object.getPosition().y - player.getSize().y);

				auto speed = (object.getType() == SceneObject::Type::kJumpBooster) ? -666.f : -833.f;
				player.setSpeedY(speed);
			}

			else if (player.getPosition().y > object.getPosition().y && player_data_.position.y >= object.getPosition().y + object.getSize().y) {
				player.setPositionY(object.getPosition().y + object.getSize().y);
				player.setSpeedY(0);
			}

			else if (player.getPosition().x > object.getPosition().x &&
				player_data_.position.x >= object.getPosition().x + object.getSize().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x + object.getSize().x);
			}

			else if (player.getPosition().x + player.getSize().x > object.getPosition().x &&
				player_data_.position.x + player.getSize().x <= object.getPosition().x + object.getSize().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x - player.getSize().x);
			}
		}
			else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kMovingPlatform)) {
				auto& player = static_cast<Player&>(*pair.first);
				auto& object = static_cast<MovingPlatform&>(*pair.second);

				if (player.getPosition().y + player.getSize().y > object.getPosition().y &&
					player_data_.position.y + player.getSize().y <= object.getPosition().y) {
					player.setPositionY(object.getPosition().y - player.getSize().y);
					player.setSpeedY(0.f);

					entities::MoveDirection player_dir = player.getDirection();
					entities::MoveDirection platform_dir = object.getDirection();

					player.move(sf::Vector2f(player.getSpeed().x / 10.f +
						object.getSpeed().x, 0.f) * dt);
					
					player.is_on_ground_ = true;
					player.setState(entities::State::kStaying);
				}

				else if (player.getPosition().y > object.getPosition().y && player_data_.position.y >= object.getPosition().y + object.getSize().y) {
					player.setPositionY(object.getPosition().y + object.getSize().y);
					player.setSpeedY(0);
				}

				else if (player.getPosition().x > object.getPosition().x &&
					player_data_.position.x >= object.getPosition().x + object.getSize().x &&
					player_data_.position.y < object.getPosition().y + object.getSize().y &&
					player_data_.position.y > object.getPosition().y - player.getSize().y) {
					player.setPositionX(object.getPosition().x + object.getSize().x);
				}

				else if (player.getPosition().x + player.getSize().x > object.getPosition().x &&
					player_data_.position.x + player.getSize().x <= object.getPosition().x &&
					player_data_.position.y < object.getPosition().y + object.getSize().y &&
					player_data_.position.y > object.getPosition().y - player.getSize().y) {
					player.setPositionX(object.getPosition().x - player.getSize().x);
				}
		}
			else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kMovingVerticallyPlatform)) {

			auto& player = static_cast<Player&>(*pair.first);
			auto& object = static_cast<MovingPlatform&>(*pair.second);

			if (player.getPosition().x > object.getPosition().x &&
				player_data_.position.x >= object.getPosition().x + object.getSize().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x + object.getSize().x);
			}

			else if (player.getPosition().x + player.getSize().x > object.getPosition().x &&
				player_data_.position.x + player.getSize().x <= object.getPosition().x &&
				player_data_.position.y < object.getPosition().y + object.getSize().y &&
				player_data_.position.y > object.getPosition().y - player.getSize().y) {
				player.setPositionX(object.getPosition().x - player.getSize().x);
			} else if (player.getPosition().y > object.getPosition().y) {
				player.setPositionY(object.getPosition().y + object.getSize().y);
				player.setSpeedY(-player.getSpeed().y / 2.f + object.getSpeed().y);
			} else {
				player.setPositionY(object.getPosition().y - player.getSize().y);
				player.setSpeedY(0.f);
				player.move(0.f, object.getSpeed().y * dt);
				player.is_on_ground_ = true;
				player.setState(entities::State::kStaying);
			}
		}
		else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kCoin)) {
			auto& player = static_cast<Player&>(*pair.first);
			auto& object = static_cast<Pickup&>(*pair.second);
			sound_player_.play(Sounds::kPickedACoin);
			object.damage(1.f);
			player.addScore(10);	
		}

		else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kExtraLife)) {
		auto& player = static_cast<Player&>(*pair.first);
		auto& object = static_cast<Pickup&>(*pair.second);
		sound_player_.play(Sounds::kPickedACoin);
		object.damage(1.f);
		player.heal(1);
		}

		else if (matchesCategories(pair, SceneObject::Type::kPlayer, SceneObject::Type::kEnemy)) {
			auto& player = static_cast<Player&>(*pair.first);
			auto& object = static_cast<Enemy&>(*pair.second);

			if (player.getPosition().y + player.getSize().y > object.getPosition().y &&
				player_data_.position.y + player.getSize().y <= object.getPosition().y) {
			
				object.damage(1.f);
				player.setSpeedY(-player.getSpeed().y);

			}
			else {
				player.damage(1.f);
				player.makeInvulnerable();
				
				if (player.getNumberOfLifes() == 0) {
					is_player_dead_ = true;
				}
				else if (music_player_.getStatus(Tracks::kLostALife) != sf::SoundSource::Playing) {
					music_player_.setLoop(false);
					music_player_.play(Tracks::kLostALife);
				}
			}

		}
	}
}

void World::updatePlayerData() {
	player_data_.lifes = player_->getNumberOfLifes();
	player_data_.max_n_of_lifes = player_->getMaxNumberOfLifes();
	player_data_.position = player_->getPosition();
	player_data_.score = player_->getScore();
	player_data_.boomerangs_left = player_->getBoomerangsLeft();
	player_data_.is_throwing = player_->isThrowing();
}

bool World::isPlayerDead() const { 
	return is_player_dead_;
}

bool World::isReachedEnd() const {
	return is_reached_end_;
}

