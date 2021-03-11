#pragma once

namespace category {
	enum class Type {
		None = 0,
		Scene = 1 << 0,
		SceneObject = 1 << 1,
		Player = 1 << 2,
		Enemy = 1 << 3,
		Pickup = 1 << 4,
		MovingPlatform = 1 << 5,
		SoundEffect = 1 << 6,
		InGameUI = 1 << 7
	};
}
