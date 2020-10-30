#pragma once

namespace entities {
	enum class MoveDirection
	{
		kNowhere,
		kRight,
		kLeft,
		kUp,
		kDown
	};

	enum class State
	{
		kStaying,
		kRunning,
		kJumping,
		kRolling,
		kSwimming,
		kClimbing,
		kSliding,
		kDead,
		kInvulnerable
	};

	enum class Type
	{
		kPlayer,
		kEnemy,
		kMovingPlatform,
		kMovingVerticallyPlatform,
		kCoin,
		kExtraLife
	};
};