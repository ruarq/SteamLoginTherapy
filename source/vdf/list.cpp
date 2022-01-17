#include "list.hpp"

namespace vdf
{

list::list(const std::vector<object*> &objects)
	: objects(objects)
{
}

list::~list()
{
	for (auto object : objects)
	{
		if (object)
		{
			delete object;
		}
	}
}

auto list::begin() -> std::vector<object*>::iterator
{
	return objects.begin();
}

auto list::end() -> std::vector<object*>::iterator
{
	return objects.end();
}

auto list::begin() const -> std::vector<object*>::const_iterator
{
	return objects.begin();
}

auto list::end() const -> std::vector<object*>::const_iterator
{
	return objects.end();
}

auto list::at(const size_t index) -> object*
{
	return objects.at(index);
}

auto list::at(const std::string &key) -> object*
{
	for (auto object : objects)
	{
		if (object->key == key)
		{
			return object;
		}
	}

	return nullptr;
}

auto list::at(const size_t index) const -> const object*
{
	return objects.at(index);
}

auto list::at(const std::string &key) const -> const object*
{
	for (auto object : objects)
	{
		if (object->key == key)
		{
			return object;
		}
	}

	return nullptr;
}

auto list::size() const -> size_t
{
	return objects.size();
}

}