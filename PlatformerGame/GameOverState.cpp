#include "GameOverState.h"
#include "StateStack.h"
#include "Utility.h"
#include "ColorIdentifiers.h"

GameOverState::GameOverState(StateStack& stack, Context context) :
	State(stack, context),
	time_(0.f), 
	game_over_text_(),
	show_game_over_(true),
	options_(),
	option_index_(0) {

	getContext().music_player->setLoop(false);
	getContext().music_player->play(Tracks::kGameOver);

	sf::Font& font = context.fonts->get(Fonts::kMain);

	sf::Vector2f scale(2.f, 2.f);

	game_over_text_.setFont(font);
	game_over_text_.setScale(scale);
	game_over_text_.setString("Game over");
	utility::centerOrigin(game_over_text_);
	game_over_text_.setPosition(context.window->getView().getSize() / 2.f);

	sf::Text play_option_;
	play_option_.setFont(font);
	play_option_.setScale(scale);
	play_option_.setString("Play again");
	utility::centerOrigin(play_option_);
	play_option_.setPosition(context.window->getView().getSize() / 2.f);
	options_.push_back(play_option_);

	sf::Text exit_option_;
	exit_option_.setFont(font);
	exit_option_.setScale(scale);
	exit_option_.setString("Main menu");	
	utility::centerOrigin(exit_option_);
	exit_option_.setPosition(play_option_.getPosition() + sf::Vector2f(0.f, 34.f) * scale);
	options_.push_back(exit_option_);
}

void GameOverState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	sf::RectangleShape darkening_shape;
	darkening_shape.setFillColor(sf::Color(0, 0, 0, 150));
	darkening_shape.setSize(sf::Vector2f(window.getSize()));

	window.draw(darkening_shape);

	if (show_game_over_) {
		window.draw(game_over_text_);
	} else {
		for (auto& option : options_)
			window.draw(option);
	}
}

bool GameOverState::update(float dt) {
	time_ += dt;

	if (time_ > 3.f)
		show_game_over_ = false;

	if (!show_game_over_) {
		for (auto& option : options_) {
			option.setFillColor(sf::Color::White);
			option.setOutlineColor(sf::Color::White);
		}

		options_[option_index_].setFillColor(colors::kPink);
		options_[option_index_].setOutlineColor(colors::kPink);
	}
	
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event) {
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (!show_game_over_) {
		switch (event.key.code) {
		case sf::Keyboard::Up:
			if (--option_index_ == -1)
				option_index_ = options_.size() - 1;
			break;
		case sf::Keyboard::Down:
			if (++option_index_ == options_.size())
				option_index_ = 0;
			break;
		case sf::Keyboard::Enter:
			if (option_index_ == (unsigned int)Options::kPlayAgain) {
				requestStackClear();
				requestStackPush(States::kGame);
			} 
			else if (option_index_ == (unsigned int)Options::kMainMenu) {
				requestStackClear();
				requestStackPush(States::kMainMenu);
			}
		}
	}

	return false;
}
