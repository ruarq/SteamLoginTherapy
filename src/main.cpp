#include <iostream>
#include <filesystem>
#include <string>
#include <getopt.h>

#include "application_info.hpp"
#include "steam_user_database.hpp"
#include "restart_steam.sh.hpp"

using namespace std::string_literals;

const auto options_info = R"(Options:
	--list, -l				List all accounts found by the program
	--version, -v			Display program version information
	--help, -h				Display this information
)";

auto get_home_dir() -> std::string
{
	return getenv("HOME");
}

auto validate_steam_path(const std::string &steam_path) -> bool
{
	// folders we need
	const std::filesystem::path sub_folders[] = {
#if defined(SLT_OS_LINUX)
		"steam"s,
		"steam/config"s
#elif defined(SLT_OS_MACOSX)
        "config"s
#endif
	};

	// files we need
	const std::filesystem::path files[] = {
		"registry.vdf"s,

#if defined(SLT_OS_LINUX)
		"steam/config/loginusers.vdf"s,
#elif defined(SLT_OS_MACOSX)
		"config/loginusers.vdf"s,
#endif
	};

	// check files
	for (const auto &file : files)
	{
		if (!std::filesystem::exists(steam_path / file))
		{
			return false;
		}
	}

	// check dirs
	for (const auto &folder : sub_folders)
	{
		if (!std::filesystem::exists(steam_path / folder))
		{
			return false;
		}
	}

	return true;
}

auto get_steam_path() -> std::string
{
    const auto home_dir = get_home_dir();

	const std::filesystem::path possible_locations[] = {
		home_dir + "/.steam"s,							// Linux
        home_dir + "/Library/Application Support/Steam"	// MacOSX
	};

	for (const auto &location : possible_locations)
	{
		if (validate_steam_path(location))
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
#if defined(SLT_OS_LINUX)
	return steam_path + "/steam/config/loginusers.vdf"s;
#elif defined(SLT_OS_MACOSX)
	return steam_path + "/config/loginusers.vdf"s;
#endif
}

auto restart_steam() -> bool
{
	const std::string filename = std::filesystem::temp_directory_path() / "restart_steam.sh"s;
	std::ofstream script(filename);

	if (!script.is_open())
	{
		std::cout << "Couldn't write file '" << filename << "'\n";
		return false;
	}

	script << restart_steam_sh << "\n";
	script.close();

	return system(("bash "s + filename).c_str()) == 0;
}

auto config_read_steam_path() -> std::string
{
	const std::string filename = get_home_dir() + "/.config/steam_login_therapy/config"s;
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cout << "Couldn't read file '" << filename << "'\n";
		return ""s; 
	}

	std::string steam_path;
	file >> steam_path;

	return steam_path;
}

auto main(int argc, char **argv) -> int
{
	auto steam_path = get_steam_path();

	if (steam_path == "")
	{
		// try to read from config
		steam_path = config_read_steam_path();

		// no config exists
		if (steam_path.empty())
		{
			std::cout << "Couldn't find steam directory. Please specify it in the configuration file\n";
			return 1;
		}

		// validate path
		if (!validate_steam_path(steam_path))
		{
			std::cout << "'" << steam_path << "' is not recognized as steam root directory. Please check your config file\n";
			return 1;
		}
	}

	steam_user_database db;
	if (!db.load(get_loginusers_path(steam_path)))
	{
		std::cout << "Couldn't load database from file '" << get_loginusers_path(steam_path) << "'\n";
		return 1;
	}

	option long_options[] = {
		{ "list",		no_argument,		nullptr, 'l' },
		{ "version",	no_argument,		nullptr, 'v' },
		{ "help",		no_argument,		nullptr, 'h' },
		{ nullptr,		0,					nullptr,  0  }
	};

	int long_opt;
	int opt;
	while ((opt = getopt_long(argc, argv, "lvh", long_options, &long_opt)) != -1)
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
				std::cout << APPNAME << " v" << VERSION_STR << " (" << std::hex << VERSION << ")\n";
				return 0;

			case '?':
				return 1;

			case 'h':
			default:
				std::cout << "Usage: " << argv[0] << " [options] account_name\n";
				std::cout << options_info;
				return 1;
		}
	}

	if (optind >= argc)
	{
		std::cout << "Expected account name after options\n";
		return 1;
	}
	
	const auto name = argv[optind];
	auto name_type = ACCOUNT_NAME;

	// find out whether name is persona name or account name
    steam_user *user = db.query(name, PERSONA_NAME);
	if (user)
	{
		name_type = PERSONA_NAME;
	}
	else
	{
		std::cout << "No account/persona name matches '" << argv[optind] << "'\n";
		return 1;
	}

	if (db.query_most_recent()->account_name == user->account_name)
	{
		std::cout << name << " is already most recent user\n";
		return 0;
	}

	auto registry = vdf::read_file(get_registry_path(steam_path));
	if (!registry)
	{
		std::cout << "Couldn't load registry file\n";
		return 1;
	}

	registry->at("Registry", "HKCU", "Software", "Valve", "Steam", "AutoLoginUser")->val() = user->account_name;
	if (!vdf::write_file(get_registry_path(steam_path), registry))
	{
		std::cout << "Couldn't write registry file\n";
		delete registry;
		return 1;
	}
	delete registry;

	if (!db.save(get_loginusers_path(steam_path)))
	{
		std::cout << "Couldn't save database to file '" << get_loginusers_path(steam_path) << "'\n";
		return 1;
	}

	if (!restart_steam())
	{
		std::cout << "Couldn't restart steam\n";
		return 1;
	}

	return 0;
}
