#pragma once

#include <SFML\Graphics.hpp>
using namespace sf;

class Animation{
public:
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
	String keyName;
	std::map <String, Animation> animationMap;
	AnimationEditor(){}
	~AnimationEditor(){}

	void addAnimation(String name, Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step);
	void setAnimation(String);
	void playAnimation();
	void pauseAnimation();
	void set_isReflex(bool);
	void shiftAnimation(float);
	void drawAnimation(RenderWindow&, int, int);
	void drawAnimation(RenderTexture&, int x, int y);
};