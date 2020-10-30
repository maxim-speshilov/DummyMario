#include "ResourceHolder.h"

template <typename Identifier, typename Resource>
void ResourceHolder<typename Identifier, typename Resource>::load(Identifier id, const std::string& filename) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	auto inserted = resource_map_.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template <typename Identifier, typename Resource>
Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) {
	auto found_resource = resource_map_.find(id);
	assert(found_resource != resource_map_.end());

	return *found_resource->second;
}

template <typename Identifier, typename Resource>
const Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) const {
	auto found_resource = resource_map_.find(id);
	assert(found_resource != resource_map_.end());

	return *found_resource->second;
}