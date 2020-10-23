#include "Animation.h"


Animation::Animation(){
}

Animation::Animation(Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step){
	currentFrame = 0;
	this->animationSpeed = animationSpeed;

	isReflex = false;
	isOn = true;

	sprite.setTexture(texture);

	for (int i = 0; i < numberOfFrames; i++){
		frames.push_back(IntRect(left + i*step, top, width, height));
		reflexFrames.push_back(IntRect(left + i*step + width, top, -width, height));
	}
}

void Animation::shift(float elapsedTime){
	if (!isOn)
		return;

	currentFrame += animationSpeed * elapsedTime;

	if (currentFrame > frames.size())
		currentFrame -= (float)frames.size();
	if (!isReflex)
		sprite.setTextureRect(frames[(int)currentFrame]);
	else
		sprite.setTextureRect(reflexFrames[(int)currentFrame]);
}

void AnimationEditor::addAnimation(String name, Texture &texture, int left, int top, int width, int height, int numberOfFrames, float animationSpeed, int step){
	animationMap[name] = Animation(texture, left, top, width, height, numberOfFrames, animationSpeed, step);
	keyName = name;
}

void AnimationEditor::setAnimation(String name){
	keyName = name;
}

void AnimationEditor::playAnimation(){
	animationMap[keyName].isOn = true;
}

void AnimationEditor::pauseAnimation(){
	animationMap[keyName].isOn = false;
}

void AnimationEditor::set_isReflex(bool boo){
	animationMap[keyName].isReflex = boo;
}

void AnimationEditor::shiftAnimation(float elapsedTime){
	animationMap[keyName].shift(elapsedTime);
}

void AnimationEditor::drawAnimation(RenderWindow &window, int x, int y){
	animationMap[keyName].sprite.setPosition(x, y);
	window.draw(animationMap[keyName].sprite);
}

void AnimationEditor::drawAnimation(RenderTexture &rt, int x, int y){
	animationMap[keyName].sprite.setPosition(x, y);
	rt.draw(animationMap[keyName].sprite);
}

void AnimationEditor::drawAnimationByName(String key_name, RenderTexture &rt, int x, int y) {
	animationMap[key_name].sprite.setPosition(x, y);
	rt.draw(animationMap[key_name].sprite);
}