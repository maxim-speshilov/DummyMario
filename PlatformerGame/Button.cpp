#include "Button.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include <SFML/Graphics/RenderTarget.hpp>

using namespace gui;

Button::Button(const FontHolder& fonts) :
	callback_(),
	normal_color_(sf::Color::White),
	selected_color_(colors::kPink),
	pressed_color_(colors::kDarkPink),
	sprite_(),
	text_("", fonts.get(Fonts::kMain)),
	size_(300.f, 30.f),
	is_toggle_(false) {

}

void Button::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	rt.draw(text_, states);
}

bool Button::isSelectable() const {
	return true;
}

void Button::select() {
	Component::select();
	utility::setColor(text_, selected_color_);
}

void Button::deselect() {
	Component::deselect();
	utility::setColor(text_, normal_color_);
}

void Button::activate() {
	Component::activate();

	if (is_toggle_)
		utility::setColor(text_, pressed_color_);

	if (callback_)
		callback_();

	if (!is_toggle_)
		deactivate();
}

void Button::deactivate() {
	Component::deactivate();

	if (is_toggle_) {
		if (isSelected())
			utility::setColor(text_, selected_color_);
		else
			utility::setColor(text_, normal_color_);
	}
}

sf::FloatRect gui::Button::getLocalBounds() const {
	return sprite_.getLocalBounds();
}

sf::FloatRect gui::Button::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}

void Button::setCallback(Callback callback) {
	callback_ = std::move(callback);
}

void Button::setString(const std::string& str) {
	text_.setString(str);
}

void Button::setToggle(bool flag) {
	is_toggle_ = flag;
}

void Button::centerOrigin() {
	sf::FloatRect bounds = text_.getLocalBounds();
	text_.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void Button::centerText() {
	text_.move((size_.x - text_.getLocalBounds().width) / 2.f, 0.f);
}

sf::Vector2f Button::getSize() const {
	return size_ * getScale();
}

sf::Vector2f gui::Button::getPosition() const {
	return getTransform().transformPoint(sprite_.getPosition());
}

void Button::handleEvent(const sf::Event& event) {

}
