#pragma once
#include "Utility.h"
#include "MenuState.h"

template<typename Options>
inline MenuState<Options>::MenuState(StateStack& stack, Context context) :
	State(stack, context),
	gui_container_(),
	callback_map_() {
}

template<typename Options>
inline void MenuState<Options>::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(gui_container_);
}

template<typename Options>
inline bool MenuState<Options>::update(float dt) {
	return true;
}

template<typename Options>
inline bool MenuState<Options>::handleEvent(const sf::Event& event) {
	gui_container_.handleEvent(event);
	return true;
}

template<typename Options>
inline void MenuState<Options>::createContainer() {

	sf::Vector2f scale(2.f, 2.f);

	for (auto pair : magic_enum::enum_entries<Options>()) {
		auto button = std::make_shared<gui::Button>(*getContext().fonts);

		button->setScale(scale);
		button->setString(utility::spacedCamelCase(std::string(pair.second.substr(1u))));
		button->setPosition(scale * sf::Vector2f(0.f, *magic_enum::enum_index(pair.first)) * sf::Vector2f(0.f, 34.f));
		button->centerText();
		setCallback(pair.first, button);

		gui_container_.pack(std::move(button));
	}

	gui_container_.setPosition(getContext().window->getView().getSize() / 2.f);
}


template<typename Options>
inline void MenuState<Options>::setCallback(Options option, gui::Button::Ptr button) {
	auto found = callback_map_.find(option);
	assert(found != callback_map_.end());
	button->setCallback(std::move(found->second));
}