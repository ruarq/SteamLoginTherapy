#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "../Log.hpp"
#include "object.hpp"
#include "value.hpp"
#include "list.hpp"

namespace vdf
{

class parser final
{
public:
	auto parse(const std::string &vdf_string) -> object*;
	auto had_error() const -> bool;

private:
	auto current() const -> char;
	auto consume() -> char;
	auto skip_whitespace() -> void;
	auto eof() const -> bool;

	auto parse_object() -> object*;
	auto parse_value() -> value*;
	auto parse_list() -> list*;

private:
	std::string source;
	size_t pos;
	bool error = false;
};

}