#pragma once
#include "Container.h"
#include "Button.h"
#include "State.h"


template<typename Options>
class MenuState : public State {
public:
	using CallbackMap = std::map<Options, gui::Button::Callback>;

public:
	MenuState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

protected:
	virtual void createContainer();
	virtual void registerCallbacks() = 0;
	virtual void setCallback(Options option, gui::Button::Ptr button);

protected:
	gui::Container gui_container_;
	CallbackMap callback_map_;
};

#include "MenuState.inl"