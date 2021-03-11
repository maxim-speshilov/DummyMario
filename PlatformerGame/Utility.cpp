#include "Utility.h"
#define _USE_MATH_DEFINES
#include <math.h>


std::string spacedCamelCase(const std::string& str) {
	std::vector<std::size_t> idx;

	for (std::string::const_iterator it = str.begin() + 1; it != str.end(); ++it)
		if (std::isupper(*it))
			idx.push_back(it - str.begin());

	if (idx.empty())
		return str;

	std::string res(str);

	for (int i = 0; i < idx.size(); ++i)
		res.insert(idx[i] + i, 1, ' ');

	return res;
}

namespace utility {

	float radian(float x) {
		return (x * M_PI / 180.f);
	}

	void centerOrigin(sf::Text& text) {
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	void centerOrigin(sf::Sprite& sprite) {
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	void centerOrigin(sf::RectangleShape& shape) {
		sf::FloatRect bounds = shape.getLocalBounds();
		shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	std::string keyToString(sf::Keyboard::Key key) {

	#define KEYTOSTRING_CASE(KEY) case sf::Keyboard::KEY: return #KEY;

		switch (key) {
			KEYTOSTRING_CASE(Unknown)
				KEYTOSTRING_CASE(A)
				KEYTOSTRING_CASE(B)
				KEYTOSTRING_CASE(C)
				KEYTOSTRING_CASE(D)
				KEYTOSTRING_CASE(E)
				KEYTOSTRING_CASE(F)
				KEYTOSTRING_CASE(G)
				KEYTOSTRING_CASE(H)
				KEYTOSTRING_CASE(I)
				KEYTOSTRING_CASE(J)
				KEYTOSTRING_CASE(K)
				KEYTOSTRING_CASE(L)
				KEYTOSTRING_CASE(M)
				KEYTOSTRING_CASE(N)
				KEYTOSTRING_CASE(O)
				KEYTOSTRING_CASE(P)
				KEYTOSTRING_CASE(Q)
				KEYTOSTRING_CASE(R)
				KEYTOSTRING_CASE(S)
				KEYTOSTRING_CASE(T)
				KEYTOSTRING_CASE(U)
				KEYTOSTRING_CASE(V)
				KEYTOSTRING_CASE(W)
				KEYTOSTRING_CASE(X)
				KEYTOSTRING_CASE(Y)
				KEYTOSTRING_CASE(Z)
				KEYTOSTRING_CASE(Num0)
				KEYTOSTRING_CASE(Num1)
				KEYTOSTRING_CASE(Num2)
				KEYTOSTRING_CASE(Num3)
				KEYTOSTRING_CASE(Num4)
				KEYTOSTRING_CASE(Num5)
				KEYTOSTRING_CASE(Num6)
				KEYTOSTRING_CASE(Num7)
				KEYTOSTRING_CASE(Num8)
				KEYTOSTRING_CASE(Num9)
				KEYTOSTRING_CASE(Escape)
				KEYTOSTRING_CASE(LControl)
				KEYTOSTRING_CASE(LShift)
				KEYTOSTRING_CASE(LAlt)
				KEYTOSTRING_CASE(LSystem)
				KEYTOSTRING_CASE(RControl)
				KEYTOSTRING_CASE(RShift)
				KEYTOSTRING_CASE(RAlt)
				KEYTOSTRING_CASE(RSystem)
				KEYTOSTRING_CASE(Menu)
				KEYTOSTRING_CASE(LBracket)
				KEYTOSTRING_CASE(RBracket)
				KEYTOSTRING_CASE(Semicolon)
				KEYTOSTRING_CASE(Comma)
				KEYTOSTRING_CASE(Period)
				KEYTOSTRING_CASE(Quote)
				KEYTOSTRING_CASE(Slash)
				KEYTOSTRING_CASE(Backslash)
				KEYTOSTRING_CASE(Tilde)
				KEYTOSTRING_CASE(Equal)
				KEYTOSTRING_CASE(Hyphen)
				KEYTOSTRING_CASE(Space)
				KEYTOSTRING_CASE(Enter)
				KEYTOSTRING_CASE(Backspace)
				KEYTOSTRING_CASE(Tab)
				KEYTOSTRING_CASE(PageUp)
				KEYTOSTRING_CASE(PageDown)
				KEYTOSTRING_CASE(End)
				KEYTOSTRING_CASE(Home)
				KEYTOSTRING_CASE(Insert)
				KEYTOSTRING_CASE(Delete)
				KEYTOSTRING_CASE(Add)
				KEYTOSTRING_CASE(Subtract)
				KEYTOSTRING_CASE(Multiply)
				KEYTOSTRING_CASE(Divide)
				KEYTOSTRING_CASE(Left)
				KEYTOSTRING_CASE(Right)
				KEYTOSTRING_CASE(Up)
				KEYTOSTRING_CASE(Down)
				KEYTOSTRING_CASE(Numpad0)
				KEYTOSTRING_CASE(Numpad1)
				KEYTOSTRING_CASE(Numpad2)
				KEYTOSTRING_CASE(Numpad3)
				KEYTOSTRING_CASE(Numpad4)
				KEYTOSTRING_CASE(Numpad5)
				KEYTOSTRING_CASE(Numpad6)
				KEYTOSTRING_CASE(Numpad7)
				KEYTOSTRING_CASE(Numpad8)
				KEYTOSTRING_CASE(Numpad9)
				KEYTOSTRING_CASE(F1)
				KEYTOSTRING_CASE(F2)
				KEYTOSTRING_CASE(F3)
				KEYTOSTRING_CASE(F4)
				KEYTOSTRING_CASE(F5)
				KEYTOSTRING_CASE(F6)
				KEYTOSTRING_CASE(F7)
				KEYTOSTRING_CASE(F8)
				KEYTOSTRING_CASE(F9)
				KEYTOSTRING_CASE(F10)
				KEYTOSTRING_CASE(F11)
				KEYTOSTRING_CASE(F12)
				KEYTOSTRING_CASE(F13)
				KEYTOSTRING_CASE(F14)
				KEYTOSTRING_CASE(F15)
				KEYTOSTRING_CASE(Pause)
		}

		return "";
	}

	sf::Keyboard::Key stringToKey(const std::string& str) {
		if (str == "Enter")
			return sf::Keyboard::Key::Enter;
		else if (str == "Backspace")
			return sf::Keyboard::Key::Backspace;
		else if (str == "Backslash")
			return sf::Keyboard::Key::Backslash;
		else if (str == "Hyphen")
			return sf::Keyboard::Key::Hyphen;
		else if (str == "Semicolon")
			return sf::Keyboard::Key::Semicolon;
		else
			return magic_enum::enum_cast<sf::Keyboard::Key>(str).value();
	}

	void drawFrame(const sf::FloatRect& rect, sf::RenderTarget& rt) {
		sf::RectangleShape rect_shape;
		rect_shape.setPosition(rect.left, rect.top);
		rect_shape.setSize(sf::Vector2f(rect.width, rect.height));
		rect_shape.setFillColor(sf::Color(0, 0, 0, 0));
		rect_shape.setOutlineColor(sf::Color::White);
		rect_shape.setOutlineThickness(2.f);
		rt.draw(rect_shape);
	}


	void setColor(sf::Text& text, const sf::Color& color) {
		text.setFillColor(color);
		text.setOutlineColor(color);
	}

	std::string spacedCamelCase(const std::string& str) {
		std::vector<std::size_t> idx;

		for (std::string::const_iterator it = str.begin() + 1; it != str.end(); ++it)
			if (std::isupper(*it))
				idx.push_back(it - str.begin());

		if (idx.empty())
			return str;

		std::string res(str);

		for (int i = 0; i < idx.size(); ++i)
			res.insert(idx[i] + i, 1, ' ');

		return res;
	}

	namespace settings {

	std::vector<std::string> getOptionsStrings(Graphics setting) {

		switch (setting) {
		case Graphics::MouseCursorVisible:
			return detail::enum_names_str<MouseCursorVisible>();
			break;
		case Graphics::VerticalSync:
			return detail::enum_names_str<VerticalSync>();
			break;
		default:
			break;
		}
	}

	std::vector<std::string> getOptionsStrings(Audio setting) {

		switch (setting) {
		case Audio::EffectsVolume:
			return detail::enum_names_str<EffectsVolume>();
			break;
		case Audio::MusicVolume:
			return detail::enum_names_str<MusicVolume>();
			break;
		default:
			break;
		}
	}

	}
}

sf::Vector2f operator*(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
	return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

sf::Vector2u operator*(const sf::Vector2u& lhs, const sf::Vector2u& rhs) {
	return sf::Vector2u(lhs.x * rhs.x, lhs.y * rhs.y);
}

sf::Vector2f operator/(const sf::Vector2f & lhs, const sf::Vector2f & rhs) {
	return sf::Vector2f(lhs.x / rhs.x, lhs.y / rhs.y);
}

