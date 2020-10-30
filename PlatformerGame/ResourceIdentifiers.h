#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace textures {
	enum class ID {
		kRunningPlayer,
		kRollingPlayer,
		kJumpingPlayer,
		kEnemy, 
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kFullHeart,
		kVoidHeart
	};
}

namespace music {
	enum class ID {
		kMainTheme,
		kLostALife
	};
}

namespace sounds {
	enum class ID {
		kJump,
		kPickedACoin
	};
}

namespace fonts {
	enum class ID {
		kMainFont
	};
}

template <typename Identifier, typename Resource>
class ResourceHolder;

typedef ResourceHolder<textures::ID, sf::Texture> TextureHolder;
typedef ResourceHolder<fonts::ID, sf::Font>	FontHolder;
typedef ResourceHolder<sounds::ID, sf::SoundBuffer> SoundBufferHolder;