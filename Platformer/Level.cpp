#include "Level.h"
#include <iostream>

string Object::getPropertyByName(string name) const{
	return properties.at(name);
}

map <string, string> Object::getProperties() const{
	return properties;
}

Size Level::getSize() const{
	return size;
}

Size Level::getTileSize() const{
	return tileSize;
}

int Level::getSpacing() const{
	return spacing;
}

vector <Object> Level::getObjectsByType(string type) const{
	vector <Object> result;

	for (Object object : objects)
		if (object.type == type)
			result.push_back(object);

	return result;
}

Object Level::getFirstObject(string type) const {
	for (Object object : objects)
		if (object.type == type)
			return object;
}

vector <Object> Level::getAllObjects() const{
	return objects;
}

void Level::draw(sf::RenderWindow &window) const{
	for (Layer layer : layers)
		for (sf::Sprite tile : layer.tileset)
			window.draw(tile);
}

void Level::draw(sf::RenderTexture &rt) const {
	for (Layer layer : layers)
		for (sf::Sprite tile : layer.tileset)
			rt.draw(tile);
}

bool Level::loadFromXmlFile(const char* filename){
	TiXmlDocument levelFile(filename);
	if (!levelFile.LoadFile())
		return false;

	TiXmlElement *map = levelFile.FirstChildElement("map");
	size = Size(atoi(map->Attribute("width")), atoi(map->Attribute("height")));
	tileSize = Size(atoi(map->Attribute("tilewidth")), atoi(map->Attribute("tileheight")));


	TiXmlElement *tileset = map->FirstChildElement("tileset");
	firstTileGID = atoi(tileset->Attribute("firstgid"));
	TiXmlDocument tilesFile("levels/tiles3.tsx");

	if (!tilesFile.LoadFile())
		return false;

	TiXmlElement *tilesetData = tilesFile.FirstChildElement("tileset");
	spacing = atoi(tilesetData->Attribute("spacing"));
	int numberOfColumns = atoi(tilesetData->Attribute("columns"));
	int numberOfRows = atoi(tilesetData->Attribute("tilecount")) / numberOfColumns;

	TiXmlElement *image = tilesetData->FirstChildElement("image");
	sf::Image tilesetImage;


	if (!tilesetImage.loadFromFile(string("levels/") + string(image->Attribute("source"))))
		throw std::logic_error("Texture file not available");

	// here you can create a mask from a color: tilesetImage.createMaskFromColor(sf::Color(R, G, B));

	tilesetTexture.loadFromImage(tilesetImage);
	tilesetTexture.setSmooth(false);

	// creating a vector from rectangles of images;

	std::vector <sf::IntRect> tileRects;

	for (int i = 0; i < numberOfRows; i++)
		for (int j = 0; j < numberOfColumns; j++)
			tileRects.push_back(sf::IntRect(j * (tileSize.first + spacing), i * (tileSize.second + spacing), tileSize.first, tileSize.second));

	// working with layers 

	TiXmlElement *layerElement = map->FirstChildElement("layer");
	while (layerElement){
		Layer layer;
		if (layerElement->Attribute("opacity"))
			layer.opacity = 255 * strtod(layerElement->Attribute("opacity"), nullptr);
		else
			layer.opacity = 255;

		TiXmlElement *dataElement = layerElement->FirstChildElement("data");

		if (dataElement == nullptr)
			throw std::logic_error("No layer data found");

		TiXmlElement *tileElement = dataElement->FirstChildElement("tile");

		for (int i = 0; i < size.second; i++)
			for (int j = 0; j < size.first; j++)
				if (tileElement){
					int tileGID = atoi(tileElement->Attribute("gid"));
					sf::Sprite sprite;

					sprite.setTexture(tilesetTexture);
					sprite.setPosition(j*tileSize.first, i*tileSize.second);
					sprite.setTextureRect(tileRects[tileGID - firstTileGID]);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					layer.tileset.push_back(sprite);
					tileElement = tileElement->NextSiblingElement("tile");
				}

		layers.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
	}

	// working with objects

	TiXmlElement *objectgroupElement;

	if (map->FirstChildElement("objectgroup") == nullptr)
		throw std::logic_error("No objects data found");
	else {
		objectgroupElement = map->FirstChildElement("objectgroup");
		while (objectgroupElement){
			TiXmlElement *objectElement = objectgroupElement->FirstChildElement("object");

			while (objectElement){
				Object object;

				object.rect = sf::FloatRect(strtod(objectElement->Attribute("x"), nullptr), strtod(objectElement->Attribute("y"), nullptr), 0, 0);
				object.sprite = sf::Sprite(tilesetTexture, sf::IntRect(0, 0, 0, 0));
				object.sprite.setPosition(object.rect.left, object.rect.top);

				if (objectElement->Attribute("name") != NULL)
					object.name = objectElement->Attribute("name");
				if (objectElement->Attribute("type") != NULL)
					object.type = objectElement->Attribute("type");
				if (objectElement->Attribute("width") != NULL)
					object.rect.width = strtod(objectElement->Attribute("width"), nullptr);
				if (objectElement->Attribute("height") != NULL)
					object.rect.height = strtod(objectElement->Attribute("height"), nullptr);

				if (objectElement->FirstChildElement("properties")){
					TiXmlElement *propertiesElement = objectElement->FirstChildElement("properties");
					TiXmlElement *propertyElement = propertiesElement->FirstChildElement("property");

					while (propertyElement){
						object.properties[propertyElement->Attribute("name")] = propertyElement->Attribute("value");
						propertyElement = propertyElement->NextSiblingElement("property");
					}

				}

				objects.push_back(object);
				objectElement = objectElement->NextSiblingElement("object");
			}

			objectgroupElement = objectgroupElement->NextSiblingElement("objectgroup");
		}
	}

	return true;
}