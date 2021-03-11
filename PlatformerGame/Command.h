#pragma once
#include <functional>
#include <cassert>
#include "Category.h"

class SceneObject;

struct Command {
	using Action = std::function<void(SceneObject&, float time)>;

	Command();

	Action action;
	unsigned int category;
};
	
template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn) {
	return [=](SceneObject& object, float dt) {
		assert(dynamic_cast<GameObject*>(&object) != nullptr);
		fn(static_cast<GameObject&>(object), dt);
	};
}
