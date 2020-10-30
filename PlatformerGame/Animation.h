#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <map>

class Animation {
	friend class AnimationPlayer;

public:
	enum class ID {
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

public:
	Animation();
	Animation(sf::Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);
	Animation(const sf::Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);
	~Animation();

	void update(float);
	void update(sf::RectangleShape&, float);

private:
	std::vector <sf::IntRect> frames_;
	std::vector <sf::IntRect> reflex_frames_;
	float current_frame_;
	float animation_speed_;
	bool is_reflex_ = true;
	bool is_on_ = true;
	sf::Sprite sprite_;
};

