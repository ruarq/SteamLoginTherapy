#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <getopt.h>

#include "application_info.hpp"
#include "steam_user_database.hpp"
#include "restart_steam.sh.hpp"

using namespace std::string_literals;

auto get_home_dir() -> std::string
{
	return getenv("HOME");
}

auto get_steam_path() -> std::string
{
	// folders we need
	const std::filesystem::path sub_folders[] = {
		"steam"s,
		"steam/config"s
	};

	// files we need
	const std::filesystem::path files[] = {
		"registry.vdf"s,
		"steam/config/loginusers.vdf"s
	};

	const std::filesystem::path possible_locations[] = {
		get_home_dir() + "/.steam"s
	};

	for (const auto &location : possible_locations)
	{
		bool requirements_met = true;

		// check files
		for (const auto &file : files)
		{
			if (!std::filesystem::exists(location / file))
			{
				requirements_met = false;
			}
		}

		// check dirs
		for (const auto &folder : sub_folders)
		{
			if (!std::filesystem::exists(location / folder))
			{
				requirements_met = false;
			}
		}

		if (requirements_met)
		{
			return location.string();
		}
	}

	return "";
}

auto get_registry_path(const std::string &steam_path) -> std::string
{
	return steam_path + "/registry.vdf"s;
}

auto get_loginusers_path(const std::string &steam_path) -> std::string
{
	return steam_path + "/steam/config/loginusers.vdf"s;
}

auto restart_steam() -> bool
{
	const auto filename = std::filesystem::temp_directory_path() / "restart_steam.sh"s;
	std::ofstream script(filename);
	script << restart_steam_sh;
	script.close();

	return system(("bash "s + filename.string()).c_str());
}

auto main(int argc, char **argv) -> int
{
	const auto steam_path = get_steam_path();

	if (steam_path == "")
	{
		std::cout << "Couldn't find steam directory. Please specify it in the configuration file.\n";
		return 1;
	}

	steam_user_database db;
	if (!db.load(steam_path))
	{
		std::cout << "Couldn't load database from path '" << steam_path << "'\n";
		return 1;
	}

	option long_options[] = {
		{ "list",		no_argument,		nullptr, 'l' },
		{ "version",	no_argument,		nullptr, 'v' },
		{ nullptr,		0,					nullptr,  0  }
	};

	int long_opt;
	int opt;
	while ((opt = getopt_long(argc, argv, "lvp:", long_options, &long_opt)) != -1)
	{
		switch (opt)
		{
			case 'l':
			{
				const auto &users = db.query_all_users();
				for (auto &user : users)
				{
					std::cout << user.account_name << " [" << user.persona_name << "]\n";
				}
				return 0;
			}

			case 'v':
				std::cout << APPNAME << " [" << argv[0] << "] " << VERSION_STR << "\n";
				return 0;

			case '?':
			default:
				// display help here
				std::cout << "Unknown option\n";
				return 1;
		}
	}

	if (optind >= argc)
	{
		std::cout << "Expected account name after options.\n";
		return 1;
	}
	
	const auto account_name = argv[optind];
	if (db.query_most_recent()->account_name != account_name)
	{
		if (!db.query(account_name, ACCOUNT_NAME))
		{
			std::cout << "No account name matches '" << argv[optind] << "'\n";
			return 1;
		}

		auto registry = vdf::read_file(get_registry_path(steam_path));
		if (!registry)
		{
			std::cout << "Couldn't load registry, which means loginuser can't be changed\n";
			return 1;
		}

		registry->at("Registry", "HKCU", "Software", "Valve", "Steam", "AutoLoginUser")->val() = account_name;
		if (!vdf::write_file(get_registry_path(steam_path), registry))
		{
			std::cout << "Couldn't write registry file.\n";
			delete registry;
			return 1;
		}
		delete registry;

		if (!restart_steam())
		{
			std::cout << "Couldn't restart steam\n";
			return 1;
		}
	}

	return 0;
}