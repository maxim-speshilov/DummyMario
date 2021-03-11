#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include "CommandQueue.h"
#include "Player.h"
#include "Category.h"

struct PlayerHandler {

	enum class Type {
		kNone = 0,
		kRight = 1 << 0,
		kLeft = 1 << 1,
		kUp = 1 << 2,
		kDown = 1 << 3, 
		kNotUpNotDown = 1 << 4,
		kNotLeftNotRight = 1 << 5,
		kNotDown = 1 << 6
	};

	PlayerHandler(Type type_, float speed_x, float speed_y);
	PlayerHandler(Type type_, sf::Vector2f speed_);

	void operator() (Player& player, float dt) const;

	sf::Vector2f speed;
	Type type;

};

class InputManager {
public:
	enum class Action {
		kMoveLeft,
		kMoveRight,
		kMoveUp,
		kMoveDown,
		kThrowBoomerang,
		kActionCount
	};

public:
	using KeyMap = std::map<sf::Keyboard::Key, Action>;
	using ActionMap = std::map<Action, Command>;

public:
	InputManager();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

	void bindActions();
	void bindKeys();
	void bindKeysFromXml(const std::string& filename);
	void saveKeyBindingsToXml(const std::string& filename);

	bool  isRealtimeAction(Action action);

private:
	sf::Vector2f key_speed_; 
	KeyMap key_binding_;
	ActionMap action_binding_;
};