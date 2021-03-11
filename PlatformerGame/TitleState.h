#pragma once
#include "State.h"
#include "Label.h"

class TitleState : public State {
public:
	TitleState(StateStack& stack, Context context);
	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	gui::Label press_any_key_;
	gui::Label game_name_;
	sf::Sprite background_sprite_;
	float effect_time_;
	bool show_text_;
	bool is_preview_ended_;
	int opacity_;
};