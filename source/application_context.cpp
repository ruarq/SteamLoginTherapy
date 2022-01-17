#include "application_context.hpp"

using namespace std::string_literals;

auto application_context::set_steam_path(const std::string &path) -> void
{
	steam_path = path;
	registry_path = steam_path / "registry.vdf"s;
	loginusers_path = steam_path / "steam"s / "config"s / "loginusers.vdf"s;
}

auto application_context::load_loginusers() -> void
{
	users.clear();

	auto loginusers = get_loginusers();
	if (!loginusers)
	{
		DEBUG_LOG("loginusers is nullptr");
		return;
	}

	for (auto user : *loginusers->at("users"))
	{
		users.push_back(steam_user(user));
	}

	delete loginusers;
}

auto application_context::save_loginusers() -> void
{
	auto loginusers = new vdf::list();
	loginusers->key = "users";

	for (const auto &user : users)
	{
		auto loginuser = new vdf::list();
		loginuser->key = std::to_string(user.steamid);
		loginuser->push_back(new vdf::value("AccountName", user.account_name));
		loginuser->push_back(new vdf::value("PersonaName", user.persona_name));
		loginuser->push_back(new vdf::value("RememberPassword", user.remember_password ? "1" : "0"));
		loginuser->push_back(new vdf::value("MostRecent", user.most_recent ? "1" : "0"));
		loginuser->push_back(new vdf::value("Timestamp", std::to_string(user.timestamp)));
		loginusers->push_back(loginuser);
	}

	auto wrapper = new vdf::list();
	wrapper->push_back(loginusers);
	set_loginusers(wrapper);

	delete wrapper;
}

auto application_context::get_registry() -> vdf::object*
{
	return vdf::read_file(registry_path);
}

auto application_context::set_registry(const vdf::object *registry_tree) -> bool
{
	return vdf::write_file(registry_path, registry_tree);
}

auto application_context::get_loginusers() -> vdf::object*
{
	return vdf::read_file(loginusers_path);
}

auto application_context::set_loginusers(const vdf::object *loginusers_tree) -> bool
{
	return vdf::write_file(loginusers_path, loginusers_tree);
}

auto application_context::loginuser_exists(const steamid_t steamid) -> bool
{
	load_loginusers();

	for (const auto &user : users)
	{
		if (user.steamid == steamid)
		{
			return true;
		}
	}

	return false;
}

auto application_context::get_all_account_names() -> std::vector<std::string>
{
	auto loginusers = get_loginusers();
	if (!loginusers)
	{
		DEBUG_LOG("loginusers is nullptr");
		return {};
	}

	std::vector<std::string> account_names;
	for (auto user : *loginusers->at("users"))
	{
		account_names.push_back(user->at("AccountName")->val());
	}

	delete loginusers;
	return account_names;
}

auto application_context::set_most_recent(const steamid_t steamid) -> bool
{
	bool account_exists = false;

	load_loginusers();
	for (auto &user : users)
	{
		if (user.steamid == steamid)
		{
			user.most_recent = true;
			account_exists = true;
		}
		else
		{
			user.most_recent = false;
		}
	}
	save_loginusers();
	
	return account_exists;
}

auto application_context::get_most_recent() -> std::string
{
	auto loginusers = get_loginusers();
	if (!loginusers)
	{
		DEBUG_LOG("loginusers is nullptr");
		return "";
	}

	for (auto user : *loginusers->at("users"))
	{
		if (user->at("MostRecent")->val() == "1")
		{
			const auto account_name = user->at("AccountName")->val();
			delete loginusers;
			return account_name;
		}
	}

	delete loginusers;
	return "";
}

auto application_context::set_auto_login_user(const steamid_t steamid) -> bool
{
	if (!loginuser_exists(steamid))
	{
		return false;
	}

	auto registry = get_registry();
	if (!registry)
	{
		DEBUG_LOG("registry is nullptr");
		return false;
	}

	registry->at("Registry", "HKCU", "Software", "Valve", "Steam", "AutoLoginUser")->val() = get_account_name_by_steamid(steamid);
	set_registry(registry);
	
	delete registry;
	
	return true;
}

auto application_context::set_logged_in_steam_account(const steamid_t steamid) -> bool
{
	return set_most_recent(steamid) && set_auto_login_user(steamid);
}

auto application_context::restart_steam() -> bool
{
	const std::string filename = std::filesystem::temp_directory_path() / "restart_steam.sh"s;
	std::ofstream file(filename);
	if (!file.is_open())
	{
		return false;
	}

	file << restart_steam_sh << "\n";
	file.close();

	// run the script
	return system(("bash "s + filename).c_str()) == 0;
}

auto application_context::get_steamid_by_account_name(const std::string &name) -> steamid_t
{
	load_loginusers();

	for (const auto &user : users)
	{
		if (user.account_name == name)
		{
			return user.steamid;
		}
	}

	DEBUG_LOG("No user with account name '" << name << "' found");
	return 0;
}

auto application_context::get_steamid_by_persona_name(const std::string &name) -> steamid_t
{
	load_loginusers();

	for (const auto &user : users)
	{
		if (user.persona_name == name)
		{
			return user.steamid;
		}
	}

	DEBUG_LOG("No user with persona name '" << name << "' found");
	return 0;
}

auto application_context::get_account_name_by_steamid(const steamid_t steamid) -> std::string
{
	load_loginusers();

	for (const auto &user : users)
	{
		if (user.steamid == steamid)
		{
			return user.account_name;
		}
	}

	DEBUG_LOG("No user with steamid '" << steamid << "' found");
	return "";
}