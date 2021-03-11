#include "ResourceHolder.h"
#include <iostream>
#include "magic_enum.hpp"

template <typename Identifier, typename Resource>
inline void ResourceHolder<typename Identifier, typename Resource>::load(Identifier id, const std::string& filename) {
	auto resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	auto inserted = resource_map_.insert_or_assign(id, std::move(resource));
	//assert(inserted.second);
}

template <typename Identifier, typename Resource>
inline Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) {
	auto found_resource = resource_map_.find(id);

	assert(found_resource != resource_map_.end());

	return *found_resource->second;
}

template <typename Identifier, typename Resource>
inline const Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) const {
	auto found_resource = resource_map_.find(id);

	assert(found_resource != resource_map_.end());

	return *found_resource->second;
}

template<typename Identifier, typename Resource>
inline void ResourceHolder<Identifier, Resource>::clear() {
	resource_map_.clear();
}
