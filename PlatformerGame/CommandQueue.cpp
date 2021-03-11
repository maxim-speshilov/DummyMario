#include "CommandQueue.h"

void CommandQueue::push(const Command & command) {
	queue_.push(command);
}

Command CommandQueue::pop() {
	Command command = queue_.front();
	queue_.pop();
	return command;
}

bool CommandQueue::isEmpty() const {
	return queue_.empty();
}

size_t CommandQueue::getSize() const {
	return queue_.size();
}
