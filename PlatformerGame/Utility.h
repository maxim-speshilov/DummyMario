#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "magic_enum.hpp"
#include "ResourceIdentifiers.h"
#include "SettingsIdentifiers.h"
#include "OptionShiftingLabel.h"
#include <memory>
#include <string>
#include <cctype>
#include <vector>

class SceneObject;

sf::Vector2f operator*(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
sf::Vector2u operator*(const sf::Vector2u& lhs, const sf::Vector2u& rhs);
sf::Vector2f operator/(const sf::Vector2f& lhs, const sf::Vector2f& rhs);

std::string spacedCamelCase(const std::string& str);

namespace utility {

	float radian(float x);

	void centerOrigin(sf::Text& text);
	void centerOrigin(sf::Sprite& sprite);
	void centerOrigin(sf::RectangleShape& shape);

	std::string keyToString(sf::Keyboard::Key key);
	sf::Keyboard::Key stringToKey(const std::string& str);

	void drawFrame(const sf::FloatRect& rect, sf::RenderTarget& rt);

	void setColor(sf::Text& text, const sf::Color& color);

	std::string spacedCamelCase(const std::string& str);

	template<typename ResourceIdentifier>
	Resources getResourceType(ResourceIdentifier id) {
		auto enum_name = magic_enum::enum_type_name<decltype(id)>();
		if (enum_name == "Textures")
			return Resources::Textures;
		else if (enum_name == "Fonts")
			return Resources::Fonts;
		else if (enum_name == "Sounds")
			return Resources::Sounds;
		else if (enum_name == "Tracks")
			return Resources::Tracks;
		else
			throw std::logic_error("Utility::getResourceType - got id of an unknow type");

	}
	namespace settings {

	namespace detail {
		template<typename E>
		constexpr auto enum_names_str() noexcept -> magic_enum::detail::enable_if_enum_t<E, std::vector<std::string>> {
			using namespace boost::adaptors;
			using namespace boost::range;

			std::vector<std::string> result;
			copy(magic_enum::enum_names<E>() | transformed([](auto name) { return static_cast<std::string>(name).substr(1u); }),
				std::back_inserter(result));

			return result;
		}
	}

	std::vector<std::string> getOptionsStrings(Graphics setting);	
	std::vector<std::string> getOptionsStrings(Audio setting);

	}
}
