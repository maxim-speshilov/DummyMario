#include "StateStack.h"
#include <iostream>

StateStack::StateStack(State::Context context) :
	stack_(),
	context_(context),
	pending_list_(), 
	factories_() {
}

void StateStack::update(float dt) {
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr) {
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw() {
	for (auto& state : stack_)
		state->draw();
}

void StateStack::handleEvent(sf::Event& event) {
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr) {
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

std::unique_ptr<State> StateStack::createState(States state_id) {
	auto found = factories_.find(state_id);
	assert(found != factories_.end());
	return found->second();
}

void StateStack::pushState(States state_id) {
	pending_list_.push_back(PendingChange(Action::Push, state_id));
}

void StateStack::popState() {
	pending_list_.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates() {
	pending_list_.push_back(PendingChange(Action::Clear));
}

void StateStack::applyPendingChanges() {
	for (auto change : pending_list_) {
		switch (change.action) {
		case Action::Push:
			stack_.push_back(createState(change.state_id));
			break;
		case Action::Pop:
			stack_.pop_back();
			break;
		case Action::Clear:
			stack_.clear();
			break;
		}
	}

	pending_list_.clear();
}

State::Context StateStack::getContext() {
	return context_;
}

bool StateStack::isEmpty() const {
	return stack_.empty();
}

std::size_t StateStack::getSize() const {
	return stack_.size();
}

StateStack::PendingChange::PendingChange(Action action_, States state_id_) : 
	action(action_),
	state_id(state_id_) {
}
