#pragma once
#include <queue>
#include "Command.h"

class CommandQueue {
public:
	void push(const Command& command);
	Command pop();
	bool isEmpty() const;

	size_t getSize() const;

private:
	std::queue<Command> queue_;
};