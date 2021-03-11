#pragma once
#include "State.h"
#include "ResourceLoader.h"
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

class LevelLoadingState : public State {
public:
	LevelLoadingState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

	void setCompletionDegree(float completion_degree);

private:
	ResourceLoader<Textures, TextureHolder> loading_task_;

	sf::Text loading_text_;
	sf::RectangleShape progress_bar_background_;
	sf::RectangleShape progress_bar_;
	int n_of_dots_;
	float effect_time_;
};