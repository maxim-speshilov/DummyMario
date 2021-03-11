#pragma once
#include "SettingsIdentifiers.h"

template<typename Type, typename Options>
class BaseSettingsState;

enum class GraphicsButtons {
	kVerticalSync,
	kCursorVisible,
	kBack,
	kApply
};

using GraphicsSettingsState = BaseSettingsState<Graphics, GraphicsButtons>;

enum class AudioButtons {
	kEffectsVolume,
	kMusicVolume,
	kBack,
	kApply
};

using AudioSettingsState = BaseSettingsState<Audio, AudioButtons>;