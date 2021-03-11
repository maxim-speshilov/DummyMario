#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "MovingPlatform.h"
#include "Pickup.h"
#include <boost/range/join.hpp>

using ObjectGroup = Scene::ObjectGroup;

Scene::Scene() {
	auto root = std::make_unique<SceneRoot>(*this);
	addObjectToGroup(ObjectGroup::Type::Auxiliary, std::move(root));

	textures_.load(Textures::kRunningPlayer, "textures/run_set.png");
	textures_.load(Textures::kRollingPlayer, "textures/rolling_set.png");
	textures_.load(Textures::kJumpingPlayer, "textures/jump_set.png");
	textures_.load(Textures::kEnemy, "textures/enemy_set.png");
	textures_.load(Textures::kBoomerang, "textures/boomerang.png");
	textures_.load(Textures::kMovingPlatform, "textures/moving_platform.png");
	textures_.load(Textures::kMovingVerticallyPlatform, "textures/moving_up_platform.png");
	textures_.load(Textures::kCoin, "textures/coin.png");
	textures_.load(Textures::kFullHeart, "textures/full_heart.png");
	textures_.load(Textures::kVoidHeart, "textures/void_heart.png");

	registerEntity<Player>(SceneObject::Type::kPlayer, textures_);
	registerEntity<Enemy>(SceneObject::Type::kEnemy, textures_);
	registerEntity<MovingPlatform>(SceneObject::Type::kMovingPlatform, textures_);
	registerEntity<MovingPlatform>(SceneObject::Type::kMovingVerticallyPlatform, textures_);
	registerEntity<Pickup>(SceneObject::Type::kCoin, textures_);
	registerEntity<Pickup>(SceneObject::Type::kExtraLife, textures_);
};

sf::Vector2f Scene::getSize() const {
	return size_;
}

sf::Vector2f Scene::getTileSize() const {
	return tile_size_;
}

void Scene::addObjectToGroup(ObjectGroup::Type type, SceneObject::Ptr object){
	object_groups_[type].objects.push_back(std::move(object));
}

void Scene::checkObjectCollisions(std::set<std::pair<SceneObject*, SceneObject*>>& collision_pairs) {
	for (auto& left : object_groups_[ObjectGroup::MapObjects].objects) {
		for (auto& right : object_groups_[ObjectGroup::Entities].objects) {
			if (collision(*left, *right)) 
				collision_pairs.insert(std::minmax(left.get(), right.get()));

			if (collision(*right, *object_groups_[ObjectGroup::Type::Player].objects.back()) && !right->isDestroyed())
				collision_pairs.insert(std::minmax(object_groups_[ObjectGroup::Type::Player].objects.back().get(), right.get()));
		}

		if (collision(*left, *object_groups_[ObjectGroup::Type::Player].objects.back()))
			collision_pairs.insert(std::minmax(left.get(), object_groups_[ObjectGroup::Type::Player].objects.back().get()));
	}

	for (auto& left : object_groups_[ObjectGroup::Pickups].objects) {
		if (collision(*left, *object_groups_[ObjectGroup::Type::Player].objects.back())) 
			collision_pairs.insert(std::minmax(left.get(), object_groups_[ObjectGroup::Type::Player].objects.back().get()));
	}

	for (auto left_it = object_groups_[ObjectGroup::Entities].objects.begin();
		left_it != object_groups_[ObjectGroup::Entities].objects.end(); ++left_it) {
		for (auto right_it = std::next(left_it);
			right_it != object_groups_[ObjectGroup::Entities].objects.end(); ++right_it) {
			if (collision(**left_it, **right_it))
				collision_pairs.insert(std::minmax((*left_it).get(), (*right_it).get()));
		}
	}
}

SceneObject::Ptr Scene::createEntity(SceneObject::Type entity_type) {
	auto found = factories_.find(entity_type);
	assert(found != factories_.end());
	return found->second(entity_type);
}

void Scene::draw(sf::RenderTarget &rt, sf::RenderStates states) const {
	rt.draw(background_sprite_, states);

	for (auto pair : magic_enum::enum_entries<ObjectGroup::Type>()) {
		if (pair.first == ObjectGroup::Type::MapObjects)
			continue;

		for (auto& object : object_groups_[pair.first].objects)
			rt.draw(*object, states);
	}
}

void Scene::drawGroup(ObjectGroup::Type type, sf::RenderTarget& rt, sf::RenderStates states) const {
	//if (type == ObjectGroup::Type::MapObjects) {
	//	rt.draw(background_sprite_, states);
	//} else {
	//	for (auto object : object_groups_[(int)type].objects)
	//		rt.draw(object);
	//}
}

void Scene::update(float dt, CommandQueue& commands) {
	for (auto pair : magic_enum::enum_entries<ObjectGroup::Type>()) {
		if (pair.first == ObjectGroup::Type::MapObjects)
			continue;

		for (auto& object : object_groups_[pair.first].objects)
			object->update(dt, commands);
	}
}

void Scene::updateGroup(ObjectGroup::Type type, float dt, CommandQueue& commands) {
	for (auto& object : object_groups_[type].objects)
		object->update(dt, commands);
}

void Scene::onCommand(const Command& command, float dt) {
	for (auto& object : boost::range::join(object_groups_[ObjectGroup::Player].objects, object_groups_[ObjectGroup::Auxiliary].objects))
		object->onCommand(command, dt);
}

void Scene::removeRemains() {
	auto remains_begin = std::remove_if(object_groups_[ObjectGroup::Type::Entities].objects.begin(), object_groups_[ObjectGroup::Type::Entities].objects.end(),
		std::mem_fn(&SceneObject::isMarkedForRemoval));
	object_groups_[ObjectGroup::Type::Entities].objects.erase(remains_begin, object_groups_[ObjectGroup::Type::Entities].objects.end());

	remains_begin = std::remove_if(object_groups_[ObjectGroup::Type::Pickups].objects.begin(), object_groups_[ObjectGroup::Type::Pickups].objects.end(),
		std::mem_fn(&SceneObject::isMarkedForRemoval));
	object_groups_[ObjectGroup::Type::Pickups].objects.erase(remains_begin, object_groups_[ObjectGroup::Type::Pickups].objects.end());
}

bool Scene::loadFromXml(const std::string &filename) {
	TiXmlDocument levelFile(filename.data());
	if (!levelFile.LoadFile())
		std::runtime_error("Scene::loadFromXml - Failed to load level file " + filename);

	TiXmlElement* map = levelFile.FirstChildElement("map");
	auto size = sf::Vector2u(std::stoi(map->Attribute("width")), std::stoi(map->Attribute("height")));
	auto tile_size = sf::Vector2f(std::stoi(map->Attribute("tilewidth")), std::stoi(map->Attribute("tileheight")));
	size_ = sf::Vector2f(std::stoi(map->Attribute("width")), std::stoi(map->Attribute("height")));
	tile_size_ = sf::Vector2f(std::stoi(map->Attribute("tilewidth")), std::stoi(map->Attribute("tileheight")));
	background_texture_.create(size.x * tile_size.x, size.y * tile_size.y);

	TiXmlElement* tileset = map->FirstChildElement("tileset");
	int kFstTileGID = std::stoi(tileset->Attribute("firstgid"));
	TiXmlDocument tilesFile("levels/tiles3.tsx");

	if (!tilesFile.LoadFile())
		std::runtime_error("Scene::loadFromXml - Failed to load tiles file");

	TiXmlElement* tilesetData = tilesFile.FirstChildElement("tileset");
	const int kSpacing = std::stoi(tilesetData->Attribute("spacing"));
	const int kNumberOfColumns = std::stoi(tilesetData->Attribute("columns"));
	const int kNumberOfRows = std::stoi(tilesetData->Attribute("tilecount")) / kNumberOfColumns;

	TiXmlElement* image = tilesetData->FirstChildElement("image");
	sf::Image tilesetImage;
	const std::string kTilesetFileName = std::string("levels/") + std::string(image->Attribute("source"));

	if (!tilesetImage.loadFromFile(kTilesetFileName))
		throw std::runtime_error("Scene::loadFromXml - texture file not available " + kTilesetFileName);

	sf::Texture kTilesetTexture; 
	kTilesetTexture.loadFromImage(tilesetImage);
	kTilesetTexture.setSmooth(false);

	std::vector<sf::IntRect> tileRects;

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
					sprite.setPosition(j*tile_size.x, i*tile_size.y);
					sprite.setTextureRect(tileRects[tile_gid - kFstTileGID]);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					background_texture_.draw(sprite);
					layer.tileset.push_back(sprite);
					tileElement = tileElement->NextSiblingElement("tile");
				}

		layers_.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
	}

	background_texture_.display();
	background_sprite_.setTexture(background_texture_.getTexture());

	/* ----- Getting objects -----*/

	if (map->FirstChildElement("objectgroup") == nullptr)
		throw std::logic_error("Scene::loadFromXml - no objects data found");
	else {
		TiXmlElement *objectgroupElement = map->FirstChildElement("objectgroup");
		while (objectgroupElement) {
			TiXmlElement *objectElement = objectgroupElement->FirstChildElement("object");

			ObjectGroup::Type group_type = magic_enum::enum_cast<ObjectGroup::Type>(objectgroupElement->Attribute("name")).value();

			while (objectElement) {
				float width = 0.f, height = 0.f;

				auto object = std::make_unique<SceneObject>();

				SceneObject::Type type;

				if (objectElement->Attribute("type") != NULL) 
					type = magic_enum::enum_cast<SceneObject::Type>("k" + std::string(objectElement->Attribute("type"))).value();

				if (group_type == ObjectGroup::Entities || group_type == ObjectGroup::Pickups)
					object = createEntity(type);

				object->type_ = type;

				if (objectElement->Attribute("id") != NULL)
					object->id_ = std::stoi(objectElement->Attribute("id"));

				if (objectElement->Attribute("width") != NULL)
					width = std::stof(objectElement->Attribute("width"), nullptr);
					
				if (objectElement->Attribute("height") != NULL)
					height = std::stof(objectElement->Attribute("height"), nullptr);

				object->rect_.setPosition(strtod(objectElement->Attribute("x"), nullptr), strtod(objectElement->Attribute("y"), nullptr));

				if (group_type == ObjectGroup::MapObjects) 
					object->rect_.setSize(sf::Vector2f(width, height));

				object_groups_[group_type].objects.push_back(std::move(object));

				objectElement = objectElement->NextSiblingElement("object");
			}

			objectgroupElement = objectgroupElement->NextSiblingElement("objectgroup");
		}
	}

	return true;
}