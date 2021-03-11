#pragma once
#include "State.h"
#include "Utility.h"
#include <map>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>


template<typename Identifier, typename Holder>
class ResourceLoader {
public:
	using LoadersMap = std::map<Identifier, std::function<void()>>;

public:
	ResourceLoader(Holder* holder);

	void loadResources();
	void loadResource(Identifier id, const std::string& filename);

	float getCompletionDegree();
	bool isFinished();
	void registerResource(Identifier id, const std::string& filename);

private:
	void run();

private:
	sf::Thread thread_;
	sf::Mutex mutex_;

	Holder* holder_;

	LoadersMap resources_loaders_;

	bool is_finished_;
	int loading_index_;
};

#include "ResourceLoader.inl"