#include "Scene.h"

using ObjectGroup = Scene::ObjectGroup;

Type stringToType(std::string str) {
	if (str == "Player")
		return Type::kPlayer;
	else if (str == "Enemy")
		return Type::kEnemy;
	else if (str == "MovingPlatform")
		return Type::kMovingPlatform;
	else if (str == "MovingVerticallyPlatform")
		return Type::kMovingVerticallyPlatform;
	else if (str == "Coin")
		return Type::kCoin;
	else if (str == "ExtraLife")
		return Type::kExtraLife;
	else if (str == "Solid")
		return Type::kSolid;
	else if (str == "EnemyBorder")
		return Type::kEnemyBorder;
	else if (str == "PlatformBorder")
		return Type::kPlatformBorder;
	else if (str == "SlopeRight")
		return Type::kSlopeRight;
	else if (str == "ExitPipe")
		return Type::kExitPipe;
	else if (str == "JumpBooster")
		return Type::kJumpBooster;
	else if (str == "JumpSuperBooster")
		return Type::kJumpSuperBooster;
	else if (str == "LeftPipe")
		return Type::kLeftPipe;
	else
		throw std::logic_error("stringToType - got unknown type");
}

ObjectGroup::Type stringToGroupType(std::string str) {
	if (str == "Entities")
		return ObjectGroup::Type::Entities;
	else if (str == "Pickups")
		return ObjectGroup::Type::Pickups;
	else if (str == "MapObjects")
		return ObjectGroup::Type::MapObjects;
	else
		throw std::logic_error("stringToGroupType - got unknown type");
}

Scene::Scene() {

};

sf::Vector2f Scene::getSize() const {
	return size_;
}

sf::Vector2f Scene::getTileSize() const {
	return tile_size_;
}


std::list <SceneObject> Scene::getObjectsByType(Type type) const {
	std::list <SceneObject> result;

	for (const SceneObject& object : objects_)
		if (object.type_ == type)
			result.push_back(object);
		
	return result;
}

SceneObject Scene::getFirstObject(Type type) const {
	auto found = std::find_if(objects_.begin(), objects_.end(), [&](SceneObject object) {
		return (object.type_ == type);
	});

	assert(found != objects_.end());
	return *found;
}

std::list<SceneObject> Scene::getObjectGroup(ObjectGroup::Type type) const {
	return object_groups_[(int)type].objects;
}

std::list<SceneObject> Scene::getObjectsExcept(ObjectGroup::Type type) const {
	std::list<SceneObject> result;
	for (auto group : object_groups_)
		if (group.type != type)
			result.insert(result.end(), group.objects.begin(), group.objects.end());
	return result;
}

std::list<SceneObject> Scene::getAllObjects() const {
	return objects_;
}

void Scene::draw(sf::RenderTarget &rt, sf::RenderStates states) const {
	rt.draw(background_sprite_);

	for (auto object : objects_)
		rt.draw(object);
}

void Scene::update(float dt) {
	for (SceneObject object : objects_)
		object.update(dt);
}


bool Scene::loadFromXml(const std::string &filename) {
	TiXmlDocument levelFile(filename.data());
	if (!levelFile.LoadFile())
		std::runtime_error("Scene::loadFromXml - Failed to load level file " + filename);

	TiXmlElement *map = levelFile.FirstChildElement("map");
	size_ = sf::Vector2f(std::stoi(map->Attribute("width")), std::stoi(map->Attribute("height")));
	tile_size_ = sf::Vector2f(std::stoi(map->Attribute("tilewidth")), std::stoi(map->Attribute("tileheight")));
	background_texture_.create(size_.x * tile_size_.x, size_.y * tile_size_.y);

	TiXmlElement *tileset = map->FirstChildElement("tileset");
	int kFstTileGID = std::stoi(tileset->Attribute("firstgid"));
	TiXmlDocument tilesFile("levels/tiles3.tsx");

	if (!tilesFile.LoadFile())
		std::runtime_error("Scene::loadFromXml - Failed to load tiles file");

	TiXmlElement *tilesetData = tilesFile.FirstChildElement("tileset");
	const int kSpacing = std::stoi(tilesetData->Attribute("spacing"));
	const int kNumberOfColumns = std::stoi(tilesetData->Attribute("columns"));
	const int kNumberOfRows = std::stoi(tilesetData->Attribute("tilecount")) / kNumberOfColumns;

	TiXmlElement *image = tilesetData->FirstChildElement("image");
	sf::Image tilesetImage;
	const std::string kTilesetFileName = std::string("levels/") + std::string(image->Attribute("source"));

	if (!tilesetImage.loadFromFile(kTilesetFileName))
		throw std::runtime_error("Scene::loadFromXml - texture file not available " + kTilesetFileName);

	sf::Texture kTilesetTexture; 
	kTilesetTexture.loadFromImage(tilesetImage);
	kTilesetTexture.setSmooth(false);

	std::vector <sf::IntRect> tileRects;

	for (int i = 0; i < kNumberOfRows; i++)
		for (int j = 0; j < kNumberOfColumns; j++)
			tileRects.push_back(sf::IntRect(j * (tile_size_.x + kSpacing), i * (tile_size_.y + kSpacing), tile_size_.x, tile_size_.y));

	/* ----- Getting background tiles -----*/

	TiXmlElement *layerElement = map->FirstChildElement("layer");
	while (layerElement) { 
		TileLayer layer;
		if (layerElement->Attribute("opacity"))
			layer.opacity = 255 * std::stof(layerElement->Attribute("opacity"), nullptr);
		else
			layer.opacity = 255;

		TiXmlElement *dataElement = layerElement->FirstChildElement("data");

		if (dataElement == nullptr)
			throw std::logic_error("Scene::loadFromXml - No layer data found");

		TiXmlElement *tileElement = dataElement->FirstChildElement("tile");

		for (int i = 0; i < size_.y; i++)
			for (int j = 0; j < size_.x; j++)
				if (tileElement){
					int tile_gid = std::stoi(tileElement->Attribute("gid"));
					sf::Sprite sprite;
					sprite.setTexture(kTilesetTexture);
					sprite.setPosition(j*tile_size_.x, i*tile_size_.y);
					sprite.setTextureRect(tileRects[tile_gid - kFstTileGID]);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					background_texture_.draw(sprite);
					layer.tileset.push_back(sprite);
					tileElement = tileElement->NextSiblingElement("tile");
				}

		layers_.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
	}

	background_sprite_.setTexture(background_texture_.getTexture());
	background_sprite_.setOrigin(0.f, background_sprite_.getLocalBounds().height);
	background_sprite_.setScale(1.f, -1.f);

	/* ----- Getting objects -----*/

	if (map->FirstChildElement("objectgroup") == nullptr)
		throw std::logic_error("Scene::loadFromXml - no objects data found");
	else {
		TiXmlElement *objectgroupElement = map->FirstChildElement("objectgroup");
		while (objectgroupElement) {
			TiXmlElement *objectElement = objectgroupElement->FirstChildElement("object");

			ObjectGroup::Type kGroupType = stringToGroupType(objectgroupElement->Attribute("name"));

			while (objectElement) {
				SceneObject object;
				float width = 0.f, height = 0.f;

				object.rect_.setPosition(strtod(objectElement->Attribute("x"), nullptr), strtod(objectElement->Attribute("y"), nullptr));

				if (objectElement->Attribute("id") != NULL)
					object.id_ = std::stoi(objectElement->Attribute("id"));

				if (objectElement->Attribute("type") != NULL)
					object.type_ = stringToType(objectElement->Attribute("type"));

				if (objectElement->Attribute("width") != NULL)
					width = std::stof(objectElement->Attribute("width"), nullptr);
					
				if (objectElement->Attribute("height") != NULL)
					height = std::stof(objectElement->Attribute("height"), nullptr);

				object.rect_.setSize(sf::Vector2f(width, height));

				if (objectElement->FirstChildElement("properties")) {
					TiXmlElement *propertiesElement = objectElement->FirstChildElement("properties");
					TiXmlElement *propertyElement = propertiesElement->FirstChildElement("property");

					while (propertyElement) {
						object.properties_[propertyElement->Attribute("name")] = propertyElement->Attribute("value");
						propertyElement = propertyElement->NextSiblingElement("property");
					}

				}

				objects_.push_back(object);
				object_groups_[(int)kGroupType].objects.push_back(object);
				objectElement = objectElement->NextSiblingElement("object");
			}

			objectgroupElement = objectgroupElement->NextSiblingElement("objectgroup");
		}
	}

	return true;
}