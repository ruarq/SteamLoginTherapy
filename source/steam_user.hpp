#pragma once

#include <string>

#include "vdf/vdf.hpp"

using steamid_t = uint64_t;

class steam_user final
{
public:
	steam_user() = default;
	explicit steam_user(const vdf::object *user_vdf);

public:
	auto from(const vdf::object *user_vdf) -> bool;
	auto to_vdf() const -> vdf::object*;

public:
	steamid_t steamid = 0;
	std::string account_name;
	std::string persona_name;
	bool remember_password = false;
	bool most_recent = false;
	time_t timestamp = 0;
};
