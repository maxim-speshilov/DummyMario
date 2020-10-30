#pragma once

#include <SFML\Graphics.hpp>
using namespace sf;

namespace animation {
	enum ID
	{
		kStaying,
		kRunning,
		kJumping,
		kFlying,
		kSpinning,
		kRolling,
		kSwimming,
		kClimbing,
		kSliding,
		kDead,
		kInvulnerable
	};

	class Animation {
	public:
		Animation();
		Animation(Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);
		~Animation();

		std::vector <IntRect> frames_;
		std::vector <IntRect> reflex_frames_;
		float current_frame_;
		float animation_speed_;
		bool is_reflex_ = true, is_on_ = true;
		Sprite sprite_;

		void shift(float);
	};

	class AnimationEditor {
	public:
		ID key_id_;
		std::map <ID, Animation> animation_map_;
		AnimationEditor() {}
		~AnimationEditor() {}

		void addAnimation(ID, Texture&, int, int, int, int, int, float, int);
		void setAnimation(ID);
		void playAnimation();
		void pauseAnimation();
		void setReflex(bool);
		void shiftAnimation(float);
		void drawAnimation(RenderWindow&, int, int);
		void drawAnimation(RenderTexture&, int, int);
		void drawAnimationByID(ID, RenderTexture&, int, int);
	};
}