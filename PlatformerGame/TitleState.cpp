#include "TitleState.h"
#include "Utility.h"
#include <iostream>


TitleState::TitleState(StateStack& stack, Context context) :
	State(stack, context),
	game_name_("Mario-Like Game", *context.fonts),
	press_any_key_("Press any key to continue", *context.fonts),
	show_text_(false),
	is_preview_ended_(false),
	effect_time_(0.f),
	opacity_(255) {

	sf::Font& font = context.fonts->get(Fonts::kMain);

	sf::Vector2f scale(2.f, 2.f);

	game_name_.setScale(scale);
	game_name_.setPosition(context.window->getView().getSize() / 2.f);

	press_any_key_.setScale(scale);
	press_any_key_.scale(0.5f, 0.5f);
	press_any_key_.setPosition((context.window->getView().getSize() - context.window->getView().getSize() / 3.75f).x,
		(context.window->getView().getSize() - context.window->getView().getSize() / 8.f).y);

}

void TitleState::draw() {
	sf::RenderWindow& window = *getContext().window;

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, opacity_));
	darkening_shape.setSize(window.getView().getSize());

	window.draw(game_name_);

	if (!is_preview_ended_)
		window.draw(darkening_shape);

	if (show_text_ && is_preview_ended_)
		window.draw(press_any_key_);
}

bool TitleState::update(float dt) {
	
	if (--opacity_ == 0) 
		is_preview_ended_ = true;
	
	if (is_preview_ended_) 
		effect_time_ += dt;

	if (effect_time_ >= 0.5f) {
		show_text_ = !show_text_;
		effect_time_ = 0.f;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed && is_preview_ended_) {
		requestStackPop();
		requestStackPush(States::kMainMenu);
	}
	
	return true;
}
