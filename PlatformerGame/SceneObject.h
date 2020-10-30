#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <map>

class SceneObject : public sf::Drawable, public sf::Transformable {
	friend class Scene;
public:
	enum class Type {
		kPlayer,
		kEnemy,
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kExtraLife,
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
	sf::RectangleShape rect_;
	Type type_;
	std::map <std::string, std::string> properties_;

public:
	SceneObject();
	std::string getPropertyByName(std::string) const;
	std::map <std::string, std::string> getProperties() const;

protected:
	virtual void update(float dt) const;
	virtual void draw(sf::RenderTarget &rt, sf::RenderStates states) const;

private:
	unsigned int id_;
};

void setPositionX(sf::Transformable& object, float x);
void setPositionY(sf::Transformable& object, float y);