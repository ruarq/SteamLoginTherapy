#pragma once

#include <string>
#include <vector>

#include "vdf/vdf.hpp"
#include "steam_user.hpp"

enum steam_user_database_query_type
{
	ACCOUNT_NAME,
	PERSONA_NAME
};

class steam_user_database final
{
public:
	auto load(const std::string &filename) -> bool;
	auto load(const vdf::object *loginusers_vdf) -> bool;

	auto save(const std::string &filename) const -> bool;
	auto get_vdf() const -> vdf::object*;

	auto query_all_users() -> std::vector<steam_user>&;

	auto query(const steamid_t steamid) -> steam_user*;
	auto query(const std::string &name, const steam_user_database_query_type query_selection) -> steam_user*;

	auto query_most_recent() -> steam_user*;

private:
	std::vector<steam_user> users;
};
