#pragma once
#include "MenuState.h"
#include "Label.h"

enum class KeyBindingsOptions {
	kMoveLeft,
	kMoveRight,
	kJump,
	kRoll,
	kBoomerang,
	kBack
};


class KeyBindingsState : public MenuState<KeyBindingsOptions> {
public:
	using BindingLabelArray = 
		std::array<gui::Label::Ptr, static_cast<std::size_t>(InputManager::Action::kActionCount)>;
	using BindingButtonArray = 
		std::array<gui::Button::Ptr, static_cast<std::size_t>(KeyBindingsOptions::kBack)>;

public:
	KeyBindingsState(StateStack& stack, Context context);

	void draw() override;
	bool update(float dt) override;
	bool handleEvent(const sf::Event& event) override;

	void updateLabels();

private:
	void createContainer() override;
	void createButtonLabel(size_t i);
	void registerCallbacks() override;
		
private:
	BindingLabelArray labels_;
	BindingButtonArray buttons_;
};