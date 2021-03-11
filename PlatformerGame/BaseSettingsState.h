#pragma once
#include "MenuState.h"
#include "Label.h"
#include "OptionShiftingLabel.h"
#include "SettingsManager.h"
#include "SettingsIdentifiers.h"
#include "magic_enum.hpp"

template<typename SettingsType, typename Options>
class BaseSettingsState : public MenuState<Options> {
public:
	using LabelArray =
		std::array<std::shared_ptr<gui::OptionShiftingLabel<SettingsType>>, magic_enum::enum_count<Options>() - 2>;
	using ButtonArray =
		std::array<gui::Button::Ptr, magic_enum::enum_count<Options>() - 2>;

public:
	BaseSettingsState(StateStack& stack, State::Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

protected:
	void createElement(size_t i);
	virtual void createContainer() override;
	virtual void registerCallbacks() override;

protected:
	LabelArray labels_;
	ButtonArray buttons_;
	gui::Container::Ptr container_;
};

#include "BaseSettingsState.inl"