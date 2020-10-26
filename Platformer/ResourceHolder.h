#pragma once
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Identifier, typename Resource>
class ResourceHolder {
private:
	std::map<Identifier, std::unique_ptr<Resource>> resource_map_;

public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
};

#include "ResourceHolder.inl"