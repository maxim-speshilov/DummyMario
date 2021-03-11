#pragma once
#include <vector>
#include "magic_enum.hpp"
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <iostream>

enum class Settings {
	Graphics,
	Audio
};

enum class Graphics {
	VerticalSync,
	MouseCursorVisible
};

enum class VerticalSync {
	kEnabled,
	kDisabled
};

enum class MouseCursorVisible {
	kNo,
	kYes
};

enum class Audio {
	EffectsVolume,
	MusicVolume
};

enum class EffectsVolume {
	k100,
	k0,
	k10,
	k20,
	k30,
	k40,
	k50,
	k60,
	k70,
	k80,
	k90
};

enum class MusicVolume {
	k100,
	k0,
	k10,
	k20,
	k30,
	k40,
	k50,
	k60,
	k70,
	k80,
	k90,
};