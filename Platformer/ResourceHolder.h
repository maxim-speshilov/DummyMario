#pragma once
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

template <typename Identifier, typename Resource>
class ResourceHolder {
private:
	std::map<Identifier, std::unique_ptr<Resource>> resource_map_;

public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

};

template <typename Identifier, typename Resource>
void ResourceHolder<typename Identifier, typename Resource>::load(Identifier id, const std::string& filename) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	resource_map_.insert(std::make_pair(id, std::move(resource)));
}

template <typename Identifier, typename Resource>
Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) {
	auto found_resource = resource_map_.find(id);
	return *found_resource->second;
}

template <typename Identifier, typename Resource>
const Resource& ResourceHolder<typename Identifier, typename Resource>::get(Identifier id) const {
	auto found_resource = resource_map_.find(id);
	return *found_resource->second;
}