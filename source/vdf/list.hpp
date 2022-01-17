#pragma once

#include <string>
#include <vector>

#include "object.hpp"

namespace vdf
{

class list final : public object
{
public:
	list(const std::vector<object*> &objects);
	~list();

public:
	auto begin() -> std::vector<object*>::iterator;
	auto end() -> std::vector<object*>::iterator;

	auto begin() const -> std::vector<object*>::const_iterator;
	auto end() const -> std::vector<object*>::const_iterator;

	auto at(const size_t index) -> object*;
	auto at(const std::string &key) -> object*;

	auto at(const size_t index) const -> const object*;
	auto at(const std::string &key) const -> const object*;

	auto size() const -> size_t;

private:
	std::vector<object*> objects;
};

}