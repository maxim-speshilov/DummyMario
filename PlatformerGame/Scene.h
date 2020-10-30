#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <list>
#include <string>
#include <map>
#include <array>
#include "SceneObject.h"
#include "tinyxml.h"



using Type = SceneObject::Type;

class Scene : public sf::Drawable, private sf::NonCopyable {

public:
	struct TileLayer {
		std::list<sf::Sprite> tileset;
		int opacity;
	};

	struct ObjectGroup {
		enum class Type {
			Entities,
			MapObjects,
			Pickups,
			GroupCount
		};

		Type type;
		std::list<SceneObject> objects;
	};

public:
	Scene();

	bool loadFromXml(const std::string &filename);

	void draw(sf::RenderTarget &render_target, sf::RenderStates states) const override;
	void update(float dt);

	std::list<SceneObject> getObjectsByType(Type type) const;
	std::list<SceneObject> getObjectGroup(ObjectGroup::Type type) const;
	std::list<SceneObject> getObjectsExcept(ObjectGroup::Type type) const;
	std::list<SceneObject> getAllObjects() const;
	SceneObject getFirstObject(Type type) const;
	sf::Vector2f getTileSize() const;
	sf::Vector2f getSize() const;

private:
	sf::Vector2f size_;
	sf::Vector2f  tile_size_;
	sf::Sprite background_sprite_;
	sf::RenderTexture background_texture_;
	std::list<TileLayer> layers_;
	std::list<SceneObject> objects_;
	std::array<ObjectGroup, (size_t)ObjectGroup::Type::GroupCount> object_groups_;
};




