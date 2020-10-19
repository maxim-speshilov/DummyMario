#include "Entity.h"

struct GameSettings{
	unsigned int timeCoef;
	std::vector <const char*> levelsFileNames;
	unsigned int levelCount;
	GameSettings(){
		timeCoef = 1200;
		levelsFileNames = { "map1.tmx", "map2.tmx" };
		levelCount = 0;
	}
};

GameSettings settings;

bool gameLoop(){

	for (auto levelFileName : settings.levelsFileNames) {
		
		bool boo = false, boo1 = false;

		Level level;

		if (!level.loadFromXmlFile(levelFileName))
			throw logic_error("Failed to open map file");


		Music main_theme;
		if (!main_theme.openFromFile("Mario_Theme.ogg"))
			throw logic_error("Failed to open music file");

		main_theme.setVolume(100);

		SoundBuffer buffer;
		if (!buffer.loadFromFile("Jump.ogg"))
			throw logic_error("Failed to open sound file");

		Sound jump;
		jump.setBuffer(buffer);
		jump.setVolume(50);

		Font font;
		font.loadFromFile("SuperMario256.ttf");
		Text money(String("0"), font);
		money.setPosition(0, 0);

		unsigned int screenWidth = 480;
		unsigned int screenHeight = level.getSize().second * level.getTileSize().second;

		RenderWindow window(VideoMode(screenWidth, screenHeight), "Platformer");

		View view;
		view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));

		Texture t1, t2, t3;
		Player player(level, { level.getObjectsByType("player").at(0).rect.left, level.getObjectsByType("player").at(0).rect.top }, 23, 28);

		t1.loadFromFile("run_set.png"); t2.loadFromFile("rolling_set.png"), t3.loadFromFile("jump_set.png");
		player.editor.addAnimation("Running", t1, 0, 0, 23, 28, 8, 0.005, 23);
		player.editor.addAnimation("Staying", t1, 0, 0, 23, 28, 1, 0, 0);
		player.editor.addAnimation("Rolling", t2, 0, 0, 22, 28, 8, 0.02, 22);
		player.editor.addAnimation("Jumping", t3, 0, 0, 24, 28, 4, 0.005, 24);

		Texture t4, t5;
		t4.loadFromFile("enemy_set.png");	
		t5.loadFromFile("moving_platform.png");

		std::list <Entity*> entities;
		std::list <Entity*>::iterator it;

		vector <Object> enemys = level.getObjectsByType("enemy");

		vector <Object> movingPlatforms = level.getObjectsByType("moving platform");

		if (enemys.size() != 0) {
			for (int i = 0; i < enemys.size(); i++) {
				Entity::MoveDirection dir;
				if (enemys[i].getPropertyByName("Direction") == "Right")
					dir = Entity::Right;
				else
					dir = Entity::Left;

				entities.push_front(new Enemy(level, Vector2f(enemys[i].rect.left, enemys[i].rect.top), 16, 16, dir));
			}
		}

		if (movingPlatforms.size() != 0) {
			for (int i = 0; i < movingPlatforms.size(); i++) {
				entities.push_front(new MovingPlatform(level, Vector2f(movingPlatforms[i].rect.left, movingPlatforms[i].rect.top), 96, 32));
			}
		}

		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->type == "enemy") {
				(*it)->editor.addAnimation("Running", t4, 0, 25, 16, 16, 2, 0.005, 16);
				(*it)->editor.addAnimation("Dead", t4, 34, 33, 16, 8, 1, 0, 0);
			}

			if ((*it)->type == "moving platform") {
				(*it)->editor.addAnimation("Moving", t5, 0, 0, 96, 32, 1, 0, 0);
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
				if ((event.type == Event::Closed))
					window.close();
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
			
			if ((player.rect.left > view.getSize().x / 2 && player.rect.left < level.getSize().first * level.getTileSize().first - view.getSize().x / 2 - 50)) {
				view.setCenter(player.rect.left + 50, screenHeight / 2);
			}
			if ((player.rect.top > view.getSize().y / 2 && player.rect.top < level.getSize().second * level.getTileSize().second - view.getSize().y / 2)) {
				view.setCenter(player.rect.left + 50, player.rect.top);
			}

			

			for (it = entities.begin(); it != entities.end();) {
				(*it)->update(time);
				if ((*it)->state == Entity::Dead)
					it = entities.erase(it);
				else
					it++;
			}

			for (it = entities.begin(); it != entities.end(); it++) {
				if (player.rect.intersects((*it)->rect)) {
					if ((*it)->type == "enemy") {
						if (player.speed.y > 0) {
							(*it)->speed.x = 0;
							player.speed.y *= (-1);
							(*it)->state = Entity::Dead;
						}
						else if ((*it)->state != Entity::Dead)
							return true;
					}
					else if ((*it)->type == "moving platform") {
						if (player.speed.y > 0) {
							player.rect.top = (*it)->rect.top - player.rect.height;
							player.speed.y = 0;
							player.rect.left += (player.speed.x + (*it)->speed.x)*time;
							player.isOnGround = true;
							player.state = Entity::Staying;
						}
						else if (player.speed.y < 0) {
							player.rect.top = (*it)->rect.top + 32;
							player.speed.y = 0;
						}
					}
				}
			}

			window.setView(view);

			window.clear(Color::Black);

			level.draw(window);	

			player.editor.drawAnimation(window, player.rect.left, player.rect.top);

			for (it = entities.begin(); it != entities.end(); it++)
				(*it)->editor.drawAnimation(window, (*it)->rect.left, (*it)->rect.top);

			window.setView(window.getDefaultView());
			window.draw(money);

			window.display();
		}
	}
}