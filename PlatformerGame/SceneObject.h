#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <map>
#include <list>
#include "Category.h"
#include "CommandQueue.h"

class SceneObject : public sf::Drawable, public sf::Transformable {
	friend class Scene;

public:
	using Ptr = std::unique_ptr<SceneObject>;

public:
	enum class Type {
		kPlayer,
		kEnemy,
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kExtraLife,
		kBoomerang,
		kSolid,
		kEnemyBorder,
		kPlatformBorder,
		kLeftPipe,
		kExitPipe,
		kJumpBooster,
		kJumpSuperBooster,
		kSlopeRight
	};

public:
	SceneObject();

	virtual void update(float dt, CommandQueue& commands);
	virtual void draw(sf::RenderTarget &rt, sf::RenderStates states) const;

	virtual void onCommand(const Command& command, float dt);

	sf::FloatRect getBoundingRect() const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;

	virtual bool isDestroyed() const;
	virtual bool isMarkedForRemoval() const;

	virtual unsigned int getCategory() const;
	Type getType() const;

	void setSize(float width, float height);
	void setPosition(sf::Vector2f& pos);
	void setPosition(float x, float y);
	void move(float offset_x, float offset_y);
	void move(sf::Vector2f offset);
	void setPositionX(float x);
	void setPositionY(float y);

protected:
	unsigned int id_;
	sf::RectangleShape rect_;
	Type type_;
};

bool collision(const SceneObject& lhs, const SceneObject& rhs);