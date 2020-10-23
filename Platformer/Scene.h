#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "tinyxml.h"

using namespace std;

struct Object{
	sf::FloatRect rect;
	sf::Sprite sprite;
	string name;
	string type;
	map <string, string> properties;

	string getPropertyByName(string) const;
	map <string, string> getProperties() const;
};

struct Layer{
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
public:
	Scene();
	bool loadFromXmlFile (const char*);
	vector <Object> getObjectsByType(string) const;
	vector <Object> getAllObjects() const;
	Object getFirstObject(string) const;
	sf::Vector2f getTileSize() const;
	sf::Vector2f getSize() const;
	int getSpacing() const;
	void draw(sf::RenderWindow&) const;
	void draw(sf::RenderTexture&) const;
};




