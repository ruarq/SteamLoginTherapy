#pragma once

#include <vector>
#include <filesystem>

#include "vdf/vdf.hpp"
#include "restart_steam.sh.hpp"

/**
 * @note really sorry for not finding a better name. This just does everything
 * and since the main file got so polluted I though those functions should get into a class
 */
class application_context final
{
public:
	auto set_steam_path(const std::string &path) -> void;

	auto get_registry() -> vdf::object*;
	auto set_registry(const vdf::object *registry_tree) -> bool;

	auto get_loginusers() -> vdf::object*;
	auto set_loginusers(const vdf::object *loginusers_tree) -> bool;

	auto set_most_recent(const std::string &account_name) -> bool;
	auto get_most_recent() -> std::string;

	auto set_auto_login_user(const std::string &account_name) -> bool;
	auto loginuser_exists(const std::string &account_name) -> bool;
	auto get_all_account_names() -> std::vector<std::string>;
	auto set_logged_in_steam_account(const std::string &account_name) -> bool;
	auto restart_steam() -> bool;

private:
	std::filesystem::path steam_path;
	std::filesystem::path registry_path;
	std::filesystem::path loginusers_path;
};