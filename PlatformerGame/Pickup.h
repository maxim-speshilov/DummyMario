#pragma once
#include "Entity.h"

class Pickup : public Entity {
public:
	Pickup(SceneObject::Type, const TextureHolder& textures);

	void update(float dt, CommandQueue& commands) override;

	void damage(float hp);
	bool isDestroyed() const override;

private:
	float hp_;
};