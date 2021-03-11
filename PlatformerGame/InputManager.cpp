#include "InputManager.h"
#include <iostream>
#include "Utility.h"
#include "Boomerang.h"

PlayerHandler::PlayerHandler(Type type_, float speed_x, float speed_y) :
	type(type_),
	speed(speed_x, speed_y) {
}

PlayerHandler::PlayerHandler(Type type_, sf::Vector2f speed_) :
	type(type_),
	speed(speed_) {
}

void PlayerHandler::operator() (Player& player, float dt) const {
	entities::State player_state = player.getState();

	switch (type) {
	case Type::kLeft:
		if (player_state == entities::State::kStaying)
			player.setState(entities::State::kRunning);
		player.setDirection(entities::MoveDirection::kLeft);
		player.setSpeedX(speed.x);
		break;
	case Type::kRight:
		if (player_state == entities::State::kStaying)
			player.setState(entities::State::kRunning);
		player.setDirection(entities::MoveDirection::kRight);
		player.setSpeedX(speed.x);
		break;
	case Type::kUp:
		if (player_state == entities::State::kSwimming) {
			player.setSpeedY(speed.y / 10.f);
		} else if (player_state != entities::State::kJumping) {
			player.setState(entities::State::kJumping);
			player.is_on_ground_ = false;
			player.setSpeedY(speed.y);
		}
		break;
	case Type::kDown:
		if (player_state == entities::State::kSwimming)
			player.setSpeedY(speed.y / 10.f);
		else if (player_state == entities::State::kRunning)
			player.setState(entities::State::kRolling);
		break;
	case Type::kNotLeftNotRight:
		if (player_state == entities::State::kRunning) 
			player.setState(entities::State::kStaying);
		player.setSpeedX(0.f);
		break;
	case Type::kNotDown:
		if (player_state == entities::State::kRolling)
			player.setState(entities::State::kRunning);
		break;
	case Type::kNotUpNotDown:
		if (player_state == entities::State::kSwimming)
			player.setSpeedY(0.f);
		break;
	default:
		break;
	}
}

InputManager::InputManager() :
	key_speed_(82.f, 542.f) {
	bindKeysFromXml("options/bindings/key_mapper.xml");
	bindActions();
}

void InputManager::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if (event.type == sf::Event::KeyPressed) {
		auto found = key_binding_.find(event.key.code);
		if (found != key_binding_.end() && !isRealtimeAction(found->second))
			commands.push(action_binding_[found->second]);
	} else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == getAssignedKey(Action::kMoveLeft) || event.key.code == getAssignedKey(Action::kMoveRight)) {
			Command stopRunning;
			stopRunning.category = (unsigned int)category::Type::Player;
			stopRunning.action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kNotLeftNotRight, 0.f, 0.f));
			commands.push(stopRunning);
		}

		if (event.key.code == getAssignedKey(Action::kMoveDown)) {
			Command stopRolling;
			stopRolling.category = (unsigned int)category::Type::Player;
			stopRolling.action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kNotDown, 0.f, 0.f));
			commands.push(stopRolling);
		}

		if (event.key.code == getAssignedKey(Action::kMoveDown) || event.key.code == getAssignedKey(Action::kMoveUp)) {
			Command stop;
			stop.category = (unsigned int)category::Type::Player;
			stop.action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kNotUpNotDown, 0.f, 0.f));
			commands.push(stop);
		}
	}
}

void InputManager::handleRealtimeInput(CommandQueue& commands) {
	for (auto key_action : key_binding_)
		if (sf::Keyboard::isKeyPressed(key_action.first) && isRealtimeAction(key_action.second))
			commands.push(action_binding_[key_action.second]);
}

void InputManager::assignKey(Action action, sf::Keyboard::Key key) {
	for (auto it = key_binding_.begin(); it != key_binding_.end();) {
		if (it->second == action)
			it = key_binding_.erase(it);
		else
			++it;
	}

	key_binding_[key] = action;
}

sf::Keyboard::Key InputManager::getAssignedKey(Action action) const {
	auto found = std::find_if(key_binding_.begin(), key_binding_.end(),
		[&](auto key_action) { return key_action.second == action; });

	if (found == key_binding_.end())
		return sf::Keyboard::Unknown;
	else
		return found->first;
}

void InputManager::bindActions() {
	action_binding_[Action::kMoveRight].action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kRight, key_speed_.x, 0.f));
	action_binding_[Action::kMoveRight].category = (unsigned int)category::Type::Player;
	action_binding_[Action::kMoveLeft].action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kLeft, -key_speed_.x, 0.f));
	action_binding_[Action::kMoveLeft].category = (unsigned int)category::Type::Player;
	action_binding_[Action::kMoveUp].action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kUp, 0.f, -key_speed_.y));
	action_binding_[Action::kMoveUp].category = (unsigned int)category::Type::Player;
	action_binding_[Action::kMoveDown].action = derivedAction<Player>(PlayerHandler(PlayerHandler::Type::kDown, 0.f, 0.f));
	action_binding_[Action::kMoveDown].category = (unsigned int)category::Type::Player;
	action_binding_[Action::kThrowBoomerang].action = derivedAction<Player>([](Player& player, float dt) {
		player.throwBoomerang();
	});
	action_binding_[Action::kThrowBoomerang].category = (unsigned int)category::Type::Player;
}

void InputManager::bindKeys() {
	key_binding_[sf::Keyboard::Right] = Action::kMoveRight;
	key_binding_[sf::Keyboard::Left] = Action::kMoveLeft;
	key_binding_[sf::Keyboard::Up] = Action::kMoveUp;
	key_binding_[sf::Keyboard::Down] = Action::kMoveDown;
	key_binding_[sf::Keyboard::Space] = Action::kThrowBoomerang;

	saveKeyBindingsToXml("options/bindings/key_mapper.xml");
}

void InputManager::bindKeysFromXml(const std::string& filename) {
	TiXmlDocument bindings(filename);

	if (!bindings.LoadFile())
		std::runtime_error("InputManager::bindKeysFromXml - Failed to load bindings file " + filename);

	TiXmlElement* root = bindings.RootElement();
	TiXmlElement* action = root->FirstChildElement();

	while (action) {
		auto action_optional = magic_enum::enum_cast<Action>(action->ValueStr());
		TiXmlElement* key = action->FirstChildElement();
		std::string key_string = key->ValueStr();
		sf::Keyboard::Key key_value;
		key_value = utility::stringToKey(key_string);
		key_binding_[key_value] = action_optional.value();
		action = action->NextSiblingElement();
	}
}

void InputManager::saveKeyBindingsToXml(const std::string& filename) {
	TiXmlDocument binding_document(filename);
	TiXmlNode* root(binding_document.InsertEndChild(TiXmlElement("key_bindings")));

	for (auto pair : key_binding_) {
		TiXmlElement action(std::string(magic_enum::enum_name<Action>(pair.second)));
		TiXmlElement key(utility::keyToString(pair.first));
		action.InsertEndChild(key);
		root->InsertEndChild(action);
	}

	binding_document.SaveFile();
 }


bool InputManager::isRealtimeAction(Action action) {
	switch (action) {
	case Action::kMoveLeft:
	case Action::kMoveRight:
	case Action::kMoveDown:
		return true;
	case Action::kMoveUp:	
	case Action::kThrowBoomerang:
	default:
		return false;
	}
}
