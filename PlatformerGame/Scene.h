#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <list>
#include <string>
#include <map>
#include <array>
#include <set>
#include <boost/range/adaptor/filtered.hpp>
#include "SceneObject.h"
#include "SceneRoot.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "EntityIdentifiers.h"
#include "tinyxml.h"
#include <iostream>
#include "Utility.h"

using Type = SceneObject::Type;

class Scene : public sf::Drawable, private sf::NonCopyable {

public:
	struct TileLayer {
		std::list<sf::Sprite> tileset;
		int opacity;
	};

	struct ObjectGroup {
		enum Type {
			Auxiliary,
			Player,
			MapObjects,
			Entities,
			Pickups
		};

		std::list<SceneObject::Ptr> objects;
	};

public:
	using GroupsArray = std::array<ObjectGroup, magic_enum::enum_count<ObjectGroup::Type>()>;
	using EntityFactoriesMap = std::map<SceneObject::Type, std::function<SceneObject::Ptr(SceneObject::Type)>>;
	using LayersList = std::list<TileLayer>;
	using ObjectsList = std::list<SceneObject::Ptr>;

public:
	Scene();

	bool loadFromXml(const std::string &filename);

	void draw(sf::RenderTarget &render_target, sf::RenderStates states) const override;
	void drawGroup(ObjectGroup::Type type, sf::RenderTarget &rt, sf::RenderStates states) const;

	void update(float dt, CommandQueue& commands);
	void updateGroup(ObjectGroup::Type type, float dt, CommandQueue& commands);

	void onCommand(const Command& command, float dt);
	void removeRemains(); 

	void addObjectToGroup(ObjectGroup::Type type, SceneObject::Ptr object);
	void checkObjectCollisions(std::set<std::pair<SceneObject*, SceneObject*>>& collision_pairs);

	template<typename T>
	inline void registerEntity(SceneObject::Type entity_type, TextureHolder& textures);
	SceneObject::Ptr createEntity(SceneObject::Type entity_type);
	
	sf::Vector2f getTileSize() const;
	sf::Vector2f getSize() const;

public:
	sf::Vector2f size_;
	sf::Vector2f tile_size_;
	sf::Sprite background_sprite_;
	sf::RenderTexture background_texture_;
	TextureHolder textures_;
	LayersList layers_;	
	GroupsArray object_groups_;
	EntityFactoriesMap factories_;
};


template<typename T>
inline void Scene::registerEntity(SceneObject::Type type, TextureHolder& textures) {
	factories_[type] = [&, this](SceneObject::Type typ) {
		return SceneObject::Ptr(new T(typ, textures));
	};
} 
