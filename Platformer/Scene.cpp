#include "Scene.h"
#include <iostream>

string Object::getPropertyByName(string name) const {
	return properties.at(name);
}

map <string, string> Object::getProperties() const {
	return properties;
}

Scene::Scene() {
};

sf::Vector2f Scene::getSize() const {
	return size;
}

sf::Vector2f Scene::getTileSize() const {
	return tileSize;
}

int Scene::getSpacing() const {
	return spacing;
}

vector <Object> Scene::getObjectsByType(string type) const {
	vector <Object> result;

	for (Object object : objects)
		if (object.type_ == type)
			result.push_back(object);

	return result;
}

Object Scene::getFirstObject(string type) const {
	for (Object object : objects)
		if (object.type_ == type)
			return object;
}

vector <Object> Scene::getAllObjects() const {
	return objects;
}

void Scene::draw(sf::RenderTexture &rt) const {
	sf::Sprite background_sprite(background_texture_.getTexture());
	background_sprite.setOrigin(0.f, background_sprite.getLocalBounds().height);
	background_sprite.setScale(1.f, -1.f);
	rt.draw(background_sprite);
}

void Scene::draw(sf::RenderWindow &window) const {
	for (Layer layer : layers)
		for (sf::Sprite tile : layer.tileset)
			window.draw(tile);
}

bool Scene::loadFromXml(const char* filename) {
	TiXmlDocument levelFile(filename);
	if (!levelFile.LoadFile())
		return false;

	TiXmlElement *map = levelFile.FirstChildElement("map");
	size = sf::Vector2f(atoi(map->Attribute("width")), atoi(map->Attribute("height")));
	tileSize = sf::Vector2f(atoi(map->Attribute("tilewidth")), atoi(map->Attribute("tileheight")));
	background_texture_.create(size.x * tileSize.x, size.y * tileSize.y);

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

	
	tilesetTexture.loadFromImage(tilesetImage);
	tilesetTexture.setSmooth(false);

	std::vector <sf::IntRect> tileRects;

	for (int i = 0; i < numberOfRows; i++)
		for (int j = 0; j < numberOfColumns; j++)
			tileRects.push_back(sf::IntRect(j * (tileSize.x + spacing), i * (tileSize.y + spacing), tileSize.x, tileSize.y));

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

		for (int i = 0; i < size.y; i++)
			for (int j = 0; j < size.x; j++)
				if (tileElement){
					int tileGID = atoi(tileElement->Attribute("gid"));
					sf::Sprite sprite;
					sprite.setTexture(tilesetTexture);
					sprite.setPosition(j*tileSize.x, i*tileSize.y);
					sprite.setTextureRect(tileRects[tileGID - firstTileGID]);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					background_texture_.draw(sprite);
					layer.tileset.push_back(sprite);
					tileElement = tileElement->NextSiblingElement("tile");
				}

		layers.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
	}


	TiXmlElement *objectgroupElement;

	if (map->FirstChildElement("objectgroup") == nullptr)
		throw std::logic_error("No objects data found");
	else {
		objectgroupElement = map->FirstChildElement("objectgroup");
		while (objectgroupElement){
			TiXmlElement *objectElement = objectgroupElement->FirstChildElement("object");

			while (objectElement){
				Object object;

				object.rect_ = sf::FloatRect(strtod(objectElement->Attribute("x"), nullptr), strtod(objectElement->Attribute("y"), nullptr), 0, 0);
				object.sprite = sf::Sprite(tilesetTexture, sf::IntRect(0, 0, 0, 0));
				object.sprite.setPosition(object.rect_.left, object.rect_.top);

				if (objectElement->Attribute("name") != NULL)
					object.name = objectElement->Attribute("name");
				if (objectElement->Attribute("type") != NULL)
					object.type_ = objectElement->Attribute("type");
				if (objectElement->Attribute("width") != NULL)
					object.rect_.width = strtod(objectElement->Attribute("width"), nullptr);
				if (objectElement->Attribute("height") != NULL)
					object.rect_.height = strtod(objectElement->Attribute("height"), nullptr);

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