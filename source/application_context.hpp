#pragma once

#include <vector>
#include <filesystem>

#include "vdf/vdf.hpp"
#include "restart_steam.sh.hpp"
#include "steam_user.hpp"

/**
 * @note really sorry for not finding a better name. This just does everything
 * and since the main file got so polluted I though those functions should get into a class
 * 
 * TODO(ruarq): refactor this entire thing. with the steam_user class we can load the loginusers once and then save them at the end.
 * no need to always reload the file when we want to change something
 */
class application_context final
{
public:
	auto set_steam_path(const std::string &path) -> void;

	auto load_loginusers() -> void;
	auto save_loginusers() -> void;

	auto get_registry() -> vdf::object*;
	auto set_registry(const vdf::object *registry_tree) -> bool;

	auto get_loginusers() -> vdf::object*;
	auto set_loginusers(const vdf::object *loginusers_tree) -> bool;

	auto set_most_recent(const steamid_t steamid) -> bool;
	auto get_most_recent() -> std::string;

	auto set_auto_login_user(const steamid_t steamid) -> bool;
	auto loginuser_exists(const steamid_t steamid) -> bool;
	auto get_all_account_names() -> std::vector<std::string>;
	auto set_logged_in_steam_account(const steamid_t steamid) -> bool;
	auto restart_steam() -> bool;

	auto get_steamid_by_account_name(const std::string &name) -> steamid_t;
	auto get_steamid_by_persona_name(const std::string &name) -> steamid_t;
	auto get_account_name_by_steamid(const steamid_t steamid) -> std::string;

private:
	std::filesystem::path steam_path;
	std::filesystem::path registry_path;
	std::filesystem::path loginusers_path;

	std::vector<steam_user> users;
};