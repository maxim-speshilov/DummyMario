#include "PreviewState.h"                                                                                                                                                                                                                                                               
#include "Utility.h"
#include <iostream>
 
PreviewState::PreviewState(StateStack& stack, Context context) :
	State(stack, context),
	author_name_("Author: Maxim Speshilov", *context.fonts),
	effect_time_(0.f),
	opacity_(255) {

	sf::Vector2f scale(2.f, 2.f);
	author_name_.setScale(scale);
	author_name_.setPosition(context.window->getView().getSize() / 2.f);
}

void PreviewState::draw() {
	sf::RenderWindow& window = *getContext().window;

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, std::fabs(opacity_)));
	darkening_shape.setSize(window.getView().getSize());

	window.draw(author_name_);
	window.draw(darkening_shape);
}

bool PreviewState::update(float dt) {
	if (--opacity_ == -255) {
		requestStackPop();
		requestStackPush(States::kTitle);
	}

	return true;
}

bool PreviewState::handleEvent(const sf::Event & event) {
	return true;
}
