#include "KeyBindingsState.h"
#include <iostream>

KeyBindingsState::KeyBindingsState(StateStack& stack, Context context) :
	MenuState(stack, context) {

	registerCallbacks();
	createContainer();
}

void KeyBindingsState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, 255));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);
	window.draw(gui_container_);
}

bool KeyBindingsState::update(float dt) {
	return false;
}

bool KeyBindingsState::handleEvent(const sf::Event& event) {
	bool is_key_binding = false;
	
	for (auto pair : magic_enum::enum_entries<KeyBindingsOptions>()) {
		if (pair.first == KeyBindingsOptions::kBack)
			break;

		auto i = static_cast<std::size_t>(*magic_enum::enum_index(pair.first));
		if (buttons_[i]->isActive()) {
			is_key_binding = true;
			if (event.type == sf::Event::KeyPressed) {
				getContext().input_manager->assignKey(magic_enum::enum_value<InputManager::Action>(i), event.key.code);
				buttons_[i]->deactivate();
			}
			break;
		}
	}

	if (is_key_binding)
		updateLabels();
	else
		gui_container_.handleEvent(event);

	return false;
}

void KeyBindingsState::updateLabels() {
	InputManager& input_manager = *getContext().input_manager;

	for (auto pair : magic_enum::enum_entries<InputManager::Action>()) {
		if (pair.first != InputManager::Action::kActionCount) {
			sf::Keyboard::Key key = input_manager.getAssignedKey(pair.first);
			labels_[static_cast<std::size_t>(pair.first)]->setString(utility::keyToString(key));
		}
	}
}

void KeyBindingsState::createContainer() {
	sf::Vector2f scale(2.f, 2.f);

	assert(magic_enum::enum_count<KeyBindingsOptions>() == magic_enum::enum_count<InputManager::Action>());

	size_t i;

	for (auto pair : magic_enum::enum_entries<KeyBindingsOptions>()) {
		i = static_cast<size_t>(*magic_enum::enum_index(pair.first));

		if (pair.first == KeyBindingsOptions::kBack)
			break;

		createButtonLabel(i);
	}

	auto back_button = std::make_shared<gui::Button>(*getContext().fonts);
	back_button->setScale(scale);
	back_button->setString(utility::spacedCamelCase(std::string(magic_enum::enum_name<KeyBindingsOptions>(KeyBindingsOptions::kBack).substr(1u))));
	back_button->setPosition(scale * (static_cast<float>(i) * sf::Vector2f(0.f, 34.f) + sf::Vector2f(0.f, 34.f)));
	setCallback(KeyBindingsOptions::kBack, back_button);
	gui_container_.pack(std::move(back_button));

	gui_container_.setPosition(getContext().window->getView().getSize() / 2.f);
}

void KeyBindingsState::createButtonLabel(size_t i) {

	sf::Vector2f scale(2.f, 2.f);

	auto button = std::make_shared<gui::Button>(*getContext().fonts);

	button->setScale(scale);
	button->setString(utility::spacedCamelCase(std::string(magic_enum::enum_name<KeyBindingsOptions>
	(magic_enum::enum_value<KeyBindingsOptions>(i))).substr(1u)));

	button->setPosition(scale * static_cast<float>(i) * sf::Vector2f(0.f, 34.f));
	button->setToggle(true);

	sf::Keyboard::Key key =
		getContext().input_manager->getAssignedKey(magic_enum::enum_value<InputManager::Action>(i));

	auto label = std::make_shared<gui::Label>("", *getContext().fonts);
	label->setScale(scale);
	label->setString(utility::keyToString(key));
	label->setPosition(button->getPosition() +
		sf::Vector2f(button->getSize().x, 0.f) + scale * sf::Vector2f(34.f, 0.f));

	buttons_[i] = std::move(button);
	labels_[i] = std::move(label);
	gui_container_.pack(buttons_[i]);
	gui_container_.pack(labels_[i]);
}

void KeyBindingsState::registerCallbacks() {
	callback_map_[KeyBindingsOptions::kBack] = [this]() {
		getContext().input_manager->saveKeyBindingsToXml("options/bindings/key_mapper.xml");
		requestStackPop();
	};
}