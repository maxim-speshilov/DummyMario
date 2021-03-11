#include "State.h"
#include "Label.h"

class PreviewState : public State {
public:
	PreviewState(StateStack& stack, Context context);

	virtual void draw() override;
	virtual bool update(float dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::Sprite background_sprite_;
	gui::Label author_name_;
	float effect_time_;
	int opacity_;
};