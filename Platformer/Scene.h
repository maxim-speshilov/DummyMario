#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "tinyxml.h"

using namespace std;

class Object{
public:
	sf::FloatRect rect_;
	sf::Sprite sprite;
	string name;
	string type_;
	map <string, string> properties;

	string getPropertyByName(string) const;
	map <string, string> getProperties() const;
};

class Layer{
public:
	vector <sf::Sprite> tileset;
	int opacity;
};

class Scene{
private:
	sf::Vector2f size, tileSize;
	int firstTileGID; 
	int spacing;
	vector <Layer> layers;
	vector <Object> objects;
	sf::Texture tilesetTexture;
	sf::RenderTexture background_texture_;
public:
	Scene();
	bool loadFromXmlFile (const char*);
	vector <Object> getObjectsByType(string) const;
	vector <Object> getAllObjects() const;
	Object getFirstObject(string) const;
	sf::Vector2f getTileSize() const;
	sf::Vector2f getSize() const;
	int getSpacing() const;
	void draw(sf::RenderTexture&) const;
	void draw(sf::RenderWindow&) const;
};




