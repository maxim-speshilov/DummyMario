#pragma once
#include <SFML/System/NonCopyable.hpp>
#include "ResourceLoader.h"

class ResourceManager : private sf::NonCopyable {
public:
 	ResourceManager();

public:
	void registerResources();
	void loadResources(Resources resource_type);
	void loadResources();
	float getLoadingCompletion();

private: 
	TextureLoader textures_loader_;
	FontLoader fonts_loader_;
};