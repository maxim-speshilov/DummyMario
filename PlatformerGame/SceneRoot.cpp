#include "SceneRoot.h"
#include "Scene.h"

SceneRoot::SceneRoot(Scene& scene) :
	scene_(&scene) {
}

SceneRoot::SceneRoot() {

}

unsigned int SceneRoot::getCategory() const {
	return (unsigned int)category::Type::Scene;
}

void SceneRoot::onCommand(const Command& command, float dt) {
	if (command.category & getCategory())
		command.action(*this, dt);
}
