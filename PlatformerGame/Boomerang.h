#pragma once
#include "Entity.h"

class Boomerang : public Entity {
public:
	Boomerang(const TextureHolder& textures);

	void update(float dt, CommandQueue& commands) override;

	bool isDestroyed() const override;
	bool isMarkedForRemoval() const override;

private:
	float travelled_distance_;
};