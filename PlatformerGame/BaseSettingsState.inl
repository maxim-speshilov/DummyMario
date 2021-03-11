#pragma once
#include "BaseSettingsState.h"

template<typename SettingsType, typename Options>
inline BaseSettingsState<SettingsType, Options>::BaseSettingsState(StateStack& stack, State::Context context) :
	MenuState<Options>(stack, context) {
	registerCallbacks();
	createContainer();
}

template<typename SettingsType, typename Options>
inline void BaseSettingsState<SettingsType, Options>::draw() {
	sf::RenderWindow& window = *MenuState<Options>::getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, 255));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);
	window.draw(MenuState<Options>::gui_container_);
}

template<typename SettingsType, typename Options>
inline bool BaseSettingsState<SettingsType, Options>::update(float dt) {
	return false;
}

template<typename SettingsType, typename Options>
inline bool BaseSettingsState<SettingsType, Options>::handleEvent(const sf::Event& event) {
	bool is_key_binding = false;

	for (size_t i = 0; i < labels_.size(); ++i) {

		if (buttons_[i]->isActive()) {
			is_key_binding = true;
			labels_[i]->activate();
			labels_[i]->updateText();
			labels_[i]->handleEvent(event);

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) {
					labels_[i]->deactivate();
					labels_[i]->updateText();
					buttons_[i]->deactivate();
				}

				break;
			}
		}
	}

	if (!is_key_binding)
		MenuState<Options>::gui_container_.handleEvent(event);

	return false;
}

template<typename SettingsType, typename Options>
inline void BaseSettingsState<SettingsType, Options>::registerCallbacks() {
	MenuState<Options>::callback_map_[magic_enum::enum_cast<Options>("kBack").value()] = [this]() {
		MenuState<Options>::requestStackPop();
	};

	MenuState<Options>::callback_map_[magic_enum::enum_cast<Options>("kApply").value()] = [this]() {
		for (size_t i = 0; i < labels_.size(); ++i) {
			MenuState<Options>::getContext().settings_manager->set(static_cast<size_t>(magic_enum::enum_cast<Settings>(magic_enum::enum_type_name<SettingsType>()).value()), i,
				labels_[i]->getSelectedIndex());
		}

		MenuState<Options>::getContext().settings_manager->saveToXml("options/settings.xml");
	};
}

template<typename SettingsType, typename Options>
inline void BaseSettingsState<SettingsType, Options>::createElement(size_t i) {
	sf::Vector2f scale(2.f, 2.f);

	auto button = std::make_shared<gui::Button>(*MenuState<Options>::getContext().fonts);

	button->setScale(scale);
	button->setString(utility::spacedCamelCase(std::string(magic_enum::enum_name<Options>
		(magic_enum::enum_value<Options>(i))).substr(1u)));

	button->setPosition(scale * static_cast<float>(i) * sf::Vector2f(0.f, 34.f));
	button->setToggle(true);

	auto label = std::make_shared<gui::OptionShiftingLabel<SettingsType>>(magic_enum::enum_value<SettingsType>(i), *MenuState<Options>::getContext().fonts);
	label->setPosition(button->getPosition() +
		sf::Vector2f(button->getSize().x, 0.f) + scale * sf::Vector2f(34.f, 0.f));
	auto shift = MenuState<Options>::getContext().settings_manager->getSetting(static_cast<size_t>(
		magic_enum::enum_cast<Settings>(magic_enum::enum_type_name<SettingsType>()).value()), i);
	label->setShift(shift);

	buttons_[i] = std::move(button);
	labels_[i] = std::move(label);

	MenuState<Options>::gui_container_.pack(buttons_[i]);
	MenuState<Options>::gui_container_.pack(labels_[i]);
}

template<typename SettingsType, typename Options>
inline void BaseSettingsState<SettingsType, Options>::createContainer() {

	size_t i;

	for (i = 0; i < magic_enum::enum_count<Options>() - 2; ++i)
		createElement(i);

	sf::Vector2f scale(2.f, 2.f);

	auto apply_button = std::make_shared<gui::Button>(*MenuState<Options>::getContext().fonts);
	apply_button->setString("Apply");
	apply_button->setScale(scale);
	apply_button->setPosition(buttons_.back()->getPosition() + scale * sf::Vector2f(0.f, 34.f) + scale * sf::Vector2f(0.f, 34.f));
	apply_button->setCallback(MenuState<Options>::callback_map_[magic_enum::enum_cast<Options>("kApply").value()]);

	auto back_button = std::make_shared<gui::Button>(*MenuState<Options>::getContext().fonts);
	back_button->setString("Back");
	back_button->setScale(scale);
	back_button->setPosition(apply_button->getPosition() + scale * sf::Vector2f(0.f, 34.f));
	back_button->setCallback(MenuState<Options>::callback_map_[magic_enum::enum_cast<Options>("kBack").value()]);

	MenuState<Options>::gui_container_.pack(std::move(apply_button));
	MenuState<Options>::gui_container_.pack(std::move(back_button));
	MenuState<Options>::gui_container_.setPosition(MenuState<Options>::getContext().window->getView().getSize() / 2.f);
}