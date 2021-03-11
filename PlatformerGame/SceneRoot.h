#pragma once
#include "SceneObject.h"

class Scene;

class SceneRoot : public SceneObject {
public:
	using Ptr = std::unique_ptr<SceneRoot>;

public:
	SceneRoot(Scene& scene);
	SceneRoot();

	unsigned int getCategory() const override;
	void onCommand(const Command& command, float dt) override;

public:
	Scene* scene_;
};