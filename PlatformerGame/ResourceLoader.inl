#include "ResourceLoader.h"

template<typename Identifier, typename Holder>
inline ResourceLoader<typename Identifier, typename Holder>::ResourceLoader(Holder* holder) :
	thread_(&ResourceLoader::run, this),
	mutex_(),
	holder_(holder),
	resources_loaders_(),
	is_finished_(false),
	loading_index_(0) {
}

template<typename Identifier, typename Holder>
inline void ResourceLoader<typename Identifier, typename Holder>::loadResource(Identifier id, const std::string& filename) {
	holder_->load(id, filename);
}

template<typename Identifier, typename Holder>
inline float ResourceLoader<Identifier, Holder>::getCompletionDegree() {
	sf::Lock lock(mutex_);
	return (float)loading_index_ / resources_loaders_.size();
}

template<typename Identifier, typename Holder>
inline bool ResourceLoader<Identifier, Holder>::isFinished() {
	sf::Lock lock(mutex_);
	return is_finished_;
}

template<typename Identifier, typename Holder>
inline void ResourceLoader<typename Identifier, typename Holder>::loadResources() {
	thread_.launch();
}

template<typename Identifier, typename Holder>
inline void ResourceLoader<typename Identifier, typename Holder>::registerResource(Identifier id, const std::string& filename) {
	resources_loaders_[id] = std::bind(&ResourceLoader::loadResource, this, id, filename);
}

template<typename Identifier, typename Holder>
inline void ResourceLoader<Identifier, Holder>::run() {
	for (auto pair : resources_loaders_) {
		{	
			sf::Lock lock(mutex_);
			pair.second();
		}
		{
			sf::Lock lock(mutex_);
			++loading_index_;
		}
	}

	{
		sf::Lock lock(mutex_);
		is_finished_ = true;
	}
}
