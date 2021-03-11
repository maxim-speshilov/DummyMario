#include "LevelLoadingState.h"
#include "ColorIdentifiers.h"
#include "Utility.h"

sf::Mutex mutex;

LevelLoadingState::LevelLoadingState(StateStack& stack, Context context) :
	State(stack, context),
	loading_task_(context.textures), 
	loading_text_(),
	progress_bar_background_(),
	progress_bar_(),
	n_of_dots_(3),
	effect_time_(0.f) {

	sf::Vector2f scale(2.f, 2.f);
	loading_text_.setFont(context.fonts->get(Fonts::kMain));
	loading_text_.setString("Loading...");
	loading_text_.setScale(scale);
	loading_text_.setFillColor(sf::Color::White);
	loading_text_.setOutlineColor(sf::Color::Black);
	utility::centerOrigin(loading_text_);
	loading_text_.setPosition(context.window->getView().getSize() / 2.f);

	progress_bar_background_.setFillColor(sf::Color::White);
	progress_bar_background_.setSize(sf::Vector2f(context.window->getView().getSize().x / 2.f, 10.f * scale.y));
	utility::centerOrigin(progress_bar_background_);
	progress_bar_background_.setPosition(loading_text_.getPosition() + sf::Vector2f(0.f, 80.f));

	progress_bar_.setFillColor(colors::kPink);
	progress_bar_.setSize(sf::Vector2f(0.f, 10.f * scale.y));
	progress_bar_.setPosition(progress_bar_background_.getGlobalBounds().left, progress_bar_background_.getGlobalBounds().top);

	loading_task_.registerResource(Textures::kRunningPlayer, "textures/run_set.png");
	loading_task_.registerResource(Textures::kRollingPlayer, "textures/rolling_set.png");
	loading_task_.registerResource(Textures::kJumpingPlayer, "textures/jump_set.png");
	loading_task_.registerResource(Textures::kEnemy, "textures/enemy_set.png");
	loading_task_.registerResource(Textures::kMovingPlatform, "textures/moving_platform.png");
	loading_task_.registerResource(Textures::kMovingVerticallyPlatform, "textures/moving_up_platform.png");
	loading_task_.registerResource(Textures::kCoin, "textures/coin.png");
	loading_task_.registerResource(Textures::kFullHeart, "textures/full_heart.png");
	loading_task_.registerResource(Textures::kVoidHeart, "textures/void_heart.png");
	loading_task_.loadResources();
}

void LevelLoadingState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	loading_text_.setString("Loading" + std::string(n_of_dots_, '.'));

	window.draw(loading_text_);
	window.draw(progress_bar_background_);
	window.draw(progress_bar_);

}

bool LevelLoadingState::update(float dt) {
	setCompletionDegree(loading_task_.getCompletionDegree());

	if (loading_task_.isFinished()) {
		requestStackPop();
		requestStackPush(States::kGame);
		requestStackPush(States::kSettingWorld);
	} else {
		effect_time_ += dt;

		if (effect_time_ > 0.5f) {
			effect_time_ = 0.f;

			if (++n_of_dots_ > 3)
				n_of_dots_ = 1;
		}
	}

	return true;
}

bool LevelLoadingState::handleEvent(const sf::Event& event) {
	return true;
}

void LevelLoadingState::setCompletionDegree(float completion_degree) {
	sf::Lock lock(mutex);

	if (completion_degree > 1.f)
		completion_degree = 1.f;
	
	progress_bar_.setSize(progress_bar_background_.getSize() * sf::Vector2f(completion_degree, 1.f));
}
