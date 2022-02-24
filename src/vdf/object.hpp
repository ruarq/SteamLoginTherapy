#pragma once

#include <string>
#include <cstddef>
#include <vector>

namespace vdf
{

class value;
class list;

class object
{
public:
	virtual ~object() = default;

public:
	auto as_value() -> value*;
	auto as_list() -> list*;

	auto as_value() const -> const value*;
	auto as_list() const -> const list*;

	auto is_value() const -> bool;
	auto is_list() const -> bool;

	auto val() -> std::string&;
	auto val() const -> const std::string&;

	auto begin() -> std::vector<object*>::iterator;
	auto end() -> std::vector<object*>::iterator;

	auto begin() const -> std::vector<object*>::const_iterator;
	auto end() const -> std::vector<object*>::const_iterator;
	
	auto at(const size_t index) -> object*;
	auto at(const std::string &key) -> object*;

	auto at(const size_t index) const -> const object*;
	auto at(const std::string &key) const -> const object*;

	template<typename T, typename U, typename... Args>
	auto at(const T &t, const U &u, const Args&... args) -> object*
	{
		if constexpr (sizeof...(args) > 0)
		{
			return at(t)->at(u)->at(args...);
		}
		else
		{
			return at(t)->at(u);
		}
	}

	template<typename T, typename U, typename... Args>
	auto at(const T &t, const U &u, const Args&... args) const -> const object*
	{
		if constexpr (sizeof...(args) > 0)
		{
			return at(t)->at(u)->at(args...);
		}
		else
		{
			return at(t)->at(u);
		}
	}

public:
	std::string key;
};

}