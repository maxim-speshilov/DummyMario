#include "Entity.h"
#include "UIWidget.h"

struct GameSettings {
	unsigned int timeCoef;
	std::vector <const char*> levelsFileNames;
	unsigned int levelCount;
	GameSettings() {
		timeCoef = 1200;
		levelsFileNames = { "levels/map1.tmx", "levels/map2.tmx" };
		levelCount = 0;
	}
};

GameSettings settings;

// TODO(me) : Make player invulnerable atfer touching an enemy.
bool gameLoop() {

	for (auto levelFileName : settings.levelsFileNames) {

		bool boo = false, boo1 = false;

		Level level;

		if (!level.loadFromXmlFile(levelFileName))
			throw logic_error("Failed to open map file");


		Music main_theme;
		if (!main_theme.openFromFile("sounds/Mario_Theme.ogg"))
			throw logic_error("Failed to open music file");

		main_theme.setVolume(80);

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

		unsigned int screen_width = 480;
		unsigned int screen_height = 600;

		RenderWindow window(VideoMode(screen_width, screen_height), "Platformer", Style::Titlebar | Style::Close | Style::Resize);
		window.setMouseCursorVisible(false);

		unsigned int level_width = level.getSize().first * level.getTileSize().first;
		unsigned int level_height = level.getSize().second * level.getTileSize().second;


		RenderTexture game_rt;
		game_rt.create(screen_width, screen_height);
		RenderTexture game_with_ui_rt;
		game_with_ui_rt.create(screen_width, screen_height);

		View view;
		view.reset(sf::FloatRect(0.f, level_height - (float)screen_height, screen_width, screen_height));

		Texture running_set, rolling_set, jumping_set;
		Player player(level, { level.getObjectsByType("player").at(0).rect.left, level.getObjectsByType("player").at(0).rect.top }, 23, 28);

		running_set.loadFromFile("textures/run_set.png"); rolling_set.loadFromFile("textures/rolling_set.png"), jumping_set.loadFromFile("textures/jump_set.png");
		player.editor.addAnimation("Running", running_set, 0, 0, 23, 28, 8, 0.005, 23);
		player.editor.addAnimation("Staying", running_set, 0, 0, 23, 28, 1, 0, 0);
		player.editor.addAnimation("Rolling", rolling_set, 0, 0, 22, 28, 8, 0.02, 22);
		player.editor.addAnimation("Jumping", jumping_set, 0, 0, 24, 28, 4, 0.005, 24);

		Texture enemy_set, moving_platform_set, coin_set;
		enemy_set.loadFromFile("textures/enemy_set.png");
		moving_platform_set.loadFromFile("textures/moving_platform.png");
		coin_set.loadFromFile("textures/coin.png");

		Font font;
		font.loadFromFile("fonts/SuperMario256.ttf");

		Score score(Vector2f(8, 8), Text(String("0"), font), 0);

		Texture full_heart, void_heart;
		Lives lives(Vector2f(screen_width - 34, 10.f), 3, 3);
		full_heart.loadFromFile("textures/full_heart.png");
		void_heart.loadFromFile("textures/void_heart.png");
		lives.editor.addAnimation("full heart", full_heart, 0, 0, 32, 32, 1, 0, 0);
		lives.editor.addAnimation("void heart", void_heart, 0, 0, 32, 32, 1, 0, 0);
		
		std::list <UIWidget*> ui_widgets;
		std::list <UIWidget*>::iterator widgets_it;

		ui_widgets.push_front(&score);
		ui_widgets.push_front(&lives);

		vector <Object> level_objects = level.getAllObjects();

		std::list <Entity*> entities;
		std::list <Entity*>::iterator entities_it;

		for (Object level_object : level.getAllObjects()) {

			if (level_object.type == "enemy") {
				MoveDirection dir;
				if (level_object.getPropertyByName("Direction") == "Right")
					dir = Right;
				else
					dir = Left;
				entities.push_front(new Enemy(level, Vector2f(level_object.rect.left, level_object.rect.top), 16, 16, dir));

			} 
			if (level_object.type == "coin") {
				entities.push_front(new Coin(level, Vector2f(level_object.rect.left, level_object.rect.top), 32, 32));
			}

			if (level_object.type == "moving platform") {

				entities.push_front(new MovingPlatform(level, Vector2f(level_object.rect.left, level_object.rect.top), 96, 32, Right));

			}
		}
	
		for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
			if ((*entities_it)->type == "enemy") {
				(*entities_it)->editor.addAnimation("Running", enemy_set, 0, 25, 16, 16, 2, 0.005, 16);
				(*entities_it)->editor.addAnimation("Dead", enemy_set, 34, 33, 16, 8, 1, 0, 0);
			}

			if ((*entities_it)->type == "moving platform") {
				(*entities_it)->editor.addAnimation("Moving", moving_platform_set, 0, 0, 96, 32, 1, 0, 0);
			}

			if ((*entities_it)->type == "coin") {
				(*entities_it)->editor.addAnimation("Spinning", coin_set, 0, 0, 32, 32, 4, 0.008, 32);
			}
		}

		main_theme.play();
		main_theme.setLoop(true);

		Clock clock;

		while (window.isOpen())
		{
			float time = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			time /= settings.timeCoef;

			Event event;
			while (window.pollEvent(event))
			{
				if ((event.type == Event::Closed)) {
					window.close();
					return false;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
				player.isKeyPressed["Left"] = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
				player.isKeyPressed["Right"] = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
				player.isKeyPressed["Up"] = true;
				if ((player.isOnGround) && jump.getStatus() != SoundSource::Playing)
					jump.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
				player.isKeyPressed["Down"] = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				return false;
			}

			if (!boo) {
				time = 0;
				boo = true;
			}

			player.update(time);

			if (player.rect.intersects(level.getFirstObject("ExitPipe").rect)) {
				settings.levelCount++;
				window.clear(Color::Black);
				break;
			}

			/* ----- View processing ----- */

			Vector2f current_view_center = view.getCenter();

			if (player.rect.left > screen_width / 2 && player.rect.left < level_width - screen_width / 2) {
				view.setCenter(player.rect.left, current_view_center.y);
			}

			current_view_center = view.getCenter();

			if ((player.rect.top > view.getSize().y / 2 && player.rect.top < level.getSize().second * level.getTileSize().second - view.getSize().y / 2)) {
				view.setCenter(current_view_center.x, player.rect.top);
			}

			/* ----- Entities updating -----*/

			for (entities_it = entities.begin(); entities_it != entities.end();) {
				(*entities_it)->update(time);
				if ((*entities_it)->state == Entity::Dead)
					entities_it = entities.erase(entities_it);
				else
					entities_it++;
			}

			/* ----- Entities processing -----*/

			for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++) {
				if (player.rect.intersects((*entities_it)->rect)) {
					if ((*entities_it)->type == "enemy") {
						if (player.speed.y > 0) {
							(*entities_it)->speed.x = 0;
							player.speed.y *= (-1);
							(*entities_it)->state = Entity::Dead;
						}
						else if ((*entities_it)->state != Entity::Dead) {
							if (lives.getCurrentLives() == 0) 
								return true;
							else
								lives.deleteLive();
						}
							
					}
					else if ((*entities_it)->type == "moving platform") {
						if (player.speed.y > 0) {
							player.rect.top = (*entities_it)->rect.top - player.rect.height;
							player.speed.y = 0;
							MoveDirection player_dir = player.getDirection();
							MoveDirection platform_dir = (*entities_it)->getDirection();

							player.rect.left += (player.speed.x / 10 + (*entities_it)->speed.x)*time;

							player.isOnGround = true;
							player.state = Entity::Staying;
						}
						else if (player.speed.y < 0) {
							player.rect.top = (*entities_it)->rect.top + 32;
							player.speed.y = 0;
						}
					}
					else if ((*entities_it)->type == "coin") {
						coin.play();
						(*entities_it)->state = Entity::Dead;
						score += 100;
					}

				}
			}

			game_rt.clear();
			game_rt.setView(view);

			level.draw(game_rt);

			player.editor.drawAnimation(game_rt, player.rect.left, player.rect.top);

			for (entities_it = entities.begin(); entities_it != entities.end(); entities_it++)
				(*entities_it)->editor.drawAnimation(game_rt, (*entities_it)->rect.left, (*entities_it)->rect.top);

			game_rt.display();

			game_with_ui_rt.clear();

			game_with_ui_rt.draw(Sprite(game_rt.getTexture()));

			for (widgets_it = ui_widgets.begin(); widgets_it != ui_widgets.end(); widgets_it++)
				(*widgets_it)->draw(game_with_ui_rt);

			game_with_ui_rt.display();

			window.clear();
			window.draw(Sprite(game_with_ui_rt.getTexture()));
			window.display();
		}
	}
}