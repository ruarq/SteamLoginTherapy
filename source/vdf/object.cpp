#include "object.hpp"

#include "../Log.hpp"
#include "value.hpp"
#include "list.hpp"

namespace vdf
{

auto object::as_value() -> value*
{
	return dynamic_cast<value*>(this);
}

auto object::as_list() -> list*
{
	return dynamic_cast<list*>(this);
}


auto object::as_value() const -> const value*
{
	return dynamic_cast<const value*>(this);
}

auto object::as_list() const -> const list*
{
	return dynamic_cast<const list*>(this);
}

auto object::is_value() const -> bool
{
	return dynamic_cast<const value*>(this);
}

auto object::is_list() const -> bool
{
	return dynamic_cast<const list*>(this);
}

auto object::val() -> std::string&
{
	return as_value()->val;
}

auto object::val() const -> const std::string&
{
	return as_value()->val;
}

auto object::begin() -> std::vector<object*>::iterator
{
	return as_list()->begin();
}

auto object::end() -> std::vector<object*>::iterator
{
	return as_list()->end();
}

auto object::begin() const -> std::vector<object*>::const_iterator
{
	return as_list()->begin();
}

auto object::end() const -> std::vector<object*>::const_iterator
{
	return as_list()->end();
}

auto object::at(const size_t index) -> object*
{
	return as_list()->at(index);
}

auto object::at(const std::string &key) -> object*
{
	return as_list()->at(key);
}

auto object::at(const size_t index) const -> const object*
{
	return as_list()->at(index);
}

auto object::at(const std::string &key) const -> const object*
{
	return as_list()->at(key);
}

}