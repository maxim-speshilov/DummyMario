#pragma once

#include <SFML\Graphics.hpp>
using namespace sf;

class Animation{
public:
	enum AnimationType 
	{
		Staying,
		Running,
		Jumping,
		Flying,
		Spinning,
		Rolling,
		Swimming,
		Climbing,
		Sliding,
		Dead,
		Invulnerable
	};

	std::vector <IntRect> frames, reflexFrames;
	float currentFrame;
	float animationSpeed;
	bool isReflex = true, isOn = true;
	Sprite sprite;

	Animation();
	Animation(Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);

	void shift(float);
};	

class AnimationEditor{
public:
	Animation::AnimationType key_type_;
	std::map <Animation::AnimationType, Animation> animationMap;
	AnimationEditor(){}
	~AnimationEditor(){}

	void addAnimation(Animation::AnimationType type, Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);
	void setAnimation(Animation::AnimationType);
	void playAnimation();
	void pauseAnimation();
	void set_isReflex(bool);
	void shiftAnimation(float);
	void drawAnimation(RenderWindow&, int, int);
	void drawAnimation(RenderTexture&, int x, int y);
	void drawAnimationByName(Animation::AnimationType, RenderTexture&, int, int);
};