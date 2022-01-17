#pragma once

#include <string>

#include "vdf/vdf.hpp"

using steamid_t = uint64_t;

class steam_user final
{
public:
	explicit steam_user(const vdf::object *user_tree);

public:
	steamid_t steamid;
	std::string account_name;
	std::string persona_name;
	bool remember_password;
	bool most_recent;
	time_t timestamp;
};
