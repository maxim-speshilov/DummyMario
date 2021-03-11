#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

enum class Resources {
	Sounds,
	Textures, 
	Tracks,
	Fonts
};

enum class Textures {
	kPreviewBackground,
	kTitleBackground,
	kButtonNormal,
	kRunningPlayer,
	kRollingPlayer,
	kJumpingPlayer,
	kEnemy,
	kBoomerang,
	kMovingPlatform,
	kMovingVerticallyPlatform,
	kCoin,
	kFullHeart,
	kVoidHeart
};


enum class Tracks {
	kMainTheme,
	kLostALife,
	kGameOver
};

enum class Sounds {
	kJump,
	kPickedACoin
};


enum class Fonts {
	kMain
};


template <typename Identifier, typename Resource>
class ResourceHolder;

using TextureHolder = ResourceHolder<Textures, sf::Texture>;
using FontHolder = ResourceHolder<Fonts, sf::Font>;
using SoundBufferHolder = ResourceHolder<Sounds, sf::SoundBuffer>;


template <typename Identifier, typename Holder>
class ResourceLoader;

using TextureLoader = ResourceLoader<Textures, TextureHolder>;
using FontLoader =  ResourceLoader<Fonts, FontHolder>;
