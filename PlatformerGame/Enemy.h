#pragma once
#include "Entity.h"

class Enemy : public Entity {
public: 
	Enemy(const TextureHolder&);
	Enemy(SceneObject::Type, const TextureHolder&);
	Enemy();

	void update(float dt, CommandQueue& commands) override;

	void damage(float hp);

	bool isDestroyed() const override;
	bool isMarkedForRemoval() const override;
	unsigned int getCategory() const override;

private:
	float hp_;
	float destruction_dt_;
	bool is_size_changed_;
};

