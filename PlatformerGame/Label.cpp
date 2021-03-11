#include "Label.h"
#include "ResourceHolder.h"
#include "Utility.h"

using namespace gui; 

Label::Label(const std::string& str, const FontHolder& fonts) :
	text_(str, fonts.get(Fonts::kMain)) {
}

Label::Label(const sf::Text& text) :
	text_(text) {
}

void Label::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	rt.draw(text_, states);
}

bool Label::isSelectable() const {
	return false;
}

void Label::centerOrigin() { 
	sf::FloatRect bounds = text_.getLocalBounds();
	text_.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void Label::setString(const std::string& str) {
	text_.setString(str);
}

void Label::setText(const sf::Text& text) {
	text_ = text;
}

sf::FloatRect gui::Label::getLocalBounds() const {
	return text_.getLocalBounds();
}

sf::FloatRect gui::Label::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}

sf::Vector2f gui::Label::getSize() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::Vector2f gui::Label::getPosition() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.left, bounds.top);
}


void gui::Label::handleEvent(const sf::Event & event) {

}
