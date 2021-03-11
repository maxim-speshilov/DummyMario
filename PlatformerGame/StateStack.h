#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <functional>
#include "State.h"

class StateStack : sf::NonCopyable {
public:
	enum class Action {
		Push,
		Pop,
		Clear
	};

public:
	StateStack(State::Context context);

	template<typename T>
	void registerState(States state_id);

	void update(float dt);
	void draw();
	void handleEvent(sf::Event& event);
	
	std::unique_ptr<State> createState(States state_id);
	void pushState(States state_id);
	void popState();
	void clearStates();

	void applyPendingChanges();

	State::Context getContext();

	bool isEmpty() const;
	std::size_t getSize() const;

private:
	struct PendingChange {
		PendingChange(Action action_, States state_id_ = States::kNone);

		Action action;
		States state_id;
	};

private:
	std::vector<std::unique_ptr<State>> stack_;
	std::vector<PendingChange> pending_list_;	
	State::Context context_;
	std::map<States, std::function<std::unique_ptr<State>()>> factories_;
};

template<typename T>
inline void StateStack::registerState(States state_id) {
	factories_[state_id] = [this]() {
		return std::unique_ptr<State>(new T(*this, context_));
	};
}
