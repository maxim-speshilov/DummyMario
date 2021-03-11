#pragma once
#include "Component.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "magic_enum.hpp"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "ColorIdentifiers.h"

namespace gui {
template<typename Options>
class OptionShiftingLabel : public Component {
public:
	using Ptr = std::shared_ptr<OptionShiftingLabel>;
	using OptionNames = std::vector<std::string>;

public:
	OptionShiftingLabel(Options type, const FontHolder& fonts);

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	
	void move(const sf::Vector2f& offset) override;
	
	bool isSelectable() const override;
	void updateText();
	void updateArrows();

	void shiftRight();
	void shiftLeft();

	void setShift(size_t shift);

	void centerOrigin();

	sf::FloatRect getLocalBounds() const override;
	sf::FloatRect getGlobalBounds() const override;
	sf::Vector2f getSize() const override;
	size_t getSelectedIndex() const;
	sf::Vector2f getPosition() const;

	void setPosition(const sf::Vector2f& position);

	void handleEvent(const sf::Event& event);

private:
	size_t selected_index_;
	OptionNames options_;
	sf::Text text_;
	sf::Color normal_color_;
	sf::Color pressed_color_;
	sf::ConvexShape left_arrow_;
	sf::ConvexShape right_arrow_;
};
}

template<typename Options>
gui::OptionShiftingLabel<Options>::OptionShiftingLabel(Options type, const FontHolder& fonts) :
	selected_index_(0),
	options_(utility::settings::getOptionsStrings(type)),
	text_(options_[selected_index_],
		fonts.get(Fonts::kMain)),
	normal_color_(sf::Color::White),
	pressed_color_(colors::kPink),
	left_arrow_(3),
	right_arrow_(3) {

	sf::Vector2f scale(2.f, 2.f);

	float kLength = 11.f;

	left_arrow_.setPoint(0, sf::Vector2f(0.f, 0.f));
	left_arrow_.setPoint(1, sf::Vector2f(-kLength, kLength));
	left_arrow_.setPoint(2, sf::Vector2f(0.f, 2 * kLength));
	left_arrow_.setFillColor(sf::Color::White);
	left_arrow_.setOutlineColor(sf::Color::White);

	right_arrow_.setPoint(0, sf::Vector2f(0.f, 0.f));
	right_arrow_.setPoint(1, sf::Vector2f(kLength, kLength));
	right_arrow_.setPoint(2, sf::Vector2f(0.f, 2 * kLength));
	right_arrow_.setFillColor(sf::Color::White);
	right_arrow_.setOutlineColor(sf::Color::White);

	left_arrow_.setScale(scale);
	right_arrow_.setScale(scale);
	text_.setScale(scale);
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	rt.draw(text_, states);
	rt.draw(left_arrow_, states);
	rt.draw(right_arrow_, states);
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::move(const sf::Vector2f& offset) {
	text_.move(offset);
	left_arrow_.move(offset);
	right_arrow_.move(offset);
}

template<typename Options>
bool gui::OptionShiftingLabel<Options>::isSelectable() const {
	return false;
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::updateText() {
	text_.setString(options_[selected_index_]);

	if (isActive())
		text_.setFillColor(pressed_color_);
	else
		text_.setFillColor(normal_color_);
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::updateArrows() {
	sf::Vector2f offset(22.f, 0.f);
	auto bounds = text_.getGlobalBounds();
	right_arrow_.setPosition(sf::Vector2f(bounds.left +
		bounds.width + offset.x, text_.getGlobalBounds().top));
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::shiftRight() {
	++selected_index_ %= options_.size();
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::shiftLeft() {
	selected_index_ = (selected_index_ + options_.size() - 1)
		% options_.size();
}

template<typename Options>
inline void gui::OptionShiftingLabel<Options>::setShift(size_t shift) {
	assert(shift < options_.size());
	selected_index_ = shift;
	updateText();
	updateArrows();
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::centerOrigin() {
	sf::FloatRect bounds = text_.getLocalBounds();
	text_.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

template<typename Options>
sf::FloatRect gui::OptionShiftingLabel<Options>::getLocalBounds() const {
	return sf::FloatRect();
}

template<typename Options>
sf::FloatRect gui::OptionShiftingLabel<Options>::getGlobalBounds() const {
	auto arrow_bounds = left_arrow_.getGlobalBounds();
	auto text_bounds = text_.getGlobalBounds();
	return sf::FloatRect(arrow_bounds.left, arrow_bounds.top, 2 * (arrow_bounds.width + 22.f) + text_bounds.width,
		std::max(text_bounds.height, arrow_bounds.height));
}

template<typename Options>
sf::Vector2f gui::OptionShiftingLabel<Options>::getSize() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

template<typename Options>
inline size_t gui::OptionShiftingLabel<Options>::getSelectedIndex() const {
	return selected_index_;
}

template<typename Options>
sf::Vector2f gui::OptionShiftingLabel<Options>::getPosition() const {
	auto bounds = getGlobalBounds();
	return sf::Vector2f(bounds.left, bounds.top);
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::setPosition(const sf::Vector2f& pos) {
	sf::Vector2f offset(22.f, 0.f);
	text_.setPosition(pos + 2.f * offset);
	auto bounds = text_.getGlobalBounds();
	left_arrow_.setPosition(sf::Vector2f(bounds.left, bounds.top) - offset);
	right_arrow_.setPosition(sf::Vector2f(bounds.left +
		bounds.width + offset.x, text_.getGlobalBounds().top));
}

template<typename Options>
void gui::OptionShiftingLabel<Options>::handleEvent(const sf::Event & event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Right:
			right_arrow_.setFillColor(colors::kPink);
			right_arrow_.setOutlineColor(colors::kPink);
			shiftRight();
			updateText();
			updateArrows();
			break;
		case sf::Keyboard::Left:
			left_arrow_.setFillColor(colors::kPink);
			left_arrow_.setOutlineColor(colors::kPink);
			shiftLeft();
			updateText();
			updateArrows();
			break;
		case sf::Keyboard::Enter:
			text_.setFillColor(colors::kPink);
			break;
		default:
			break;
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Right:
			right_arrow_.setFillColor(sf::Color::White);
			right_arrow_.setOutlineColor(sf::Color::White);
			break;
		case sf::Keyboard::Left:
			left_arrow_.setFillColor(sf::Color::White);
			left_arrow_.setOutlineColor(sf::Color::White);
			break;
		default:
			break;
		}
	}
}

//#include "OptionShiftingLabel.inl"