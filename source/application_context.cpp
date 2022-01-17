#include "application_context.hpp"

using namespace std::string_literals;

auto application_context::set_steam_path(const std::string &path) -> void
{
	steam_path = path;
	registry_path = steam_path / "registry.vdf"s;
	loginusers_path = steam_path / "steam"s / "config"s / "loginusers.vdf"s;
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

auto application_context::loginuser_exists(const std::string &account_name) -> bool
{
	auto loginusers = get_loginusers();
	if (!loginusers)
	{
		DEBUG_LOG("loginusers is nullptr");
		return false;
	}

	for (auto user : *loginusers->at("users"))
	{
		if (user->at("AccountName")->val() == account_name)
		{
			delete loginusers;
			return true;
		}
	}

	delete loginusers;
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

auto application_context::set_most_recent(const std::string &account_name) -> bool
{
	bool account_exists = false;

	auto loginusers = get_loginusers();
	if (!loginusers)
	{
		DEBUG_LOG("loginusers is nullptr");
		return false;
	}

	for (auto user : *loginusers->at("users"))
	{
		if (user->at("AccountName")->val() == account_name)
		{
			user->at("MostRecent")->val() = "1";
			account_exists = true;
		}
		else
		{
			user->at("MostRecent")->val() = "0";
		}
	}
	set_loginusers(loginusers);
	
	delete loginusers;

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

auto application_context::set_auto_login_user(const std::string &account_name) -> bool
{
	if (!loginuser_exists(account_name))
	{
		return false;
	}

	auto registry = get_registry();
	if (!registry)
	{
		DEBUG_LOG("registry is nullptr");
		return false;
	}

	registry->at("Registry", "HKCU", "Software", "Valve", "Steam", "AutoLoginUser")->val() = account_name;
	set_registry(registry);
	
	delete registry;
	
	return true;
}

auto application_context::set_logged_in_steam_account(const std::string &account_name) -> bool
{
	return set_most_recent(account_name) && set_auto_login_user(account_name);
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