#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <getopt.h>

#include "application_context.hpp"
#include "application_config.hpp"

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

int main(int argc, char **argv)
{
	application_context app_ctx;

	auto steam_path = get_steam_path();
	if (steam_path == "")
	{
		std::cout << "Couldn't find steam directory. Please specify using the --steam-path option.\n";
		return 1;
	}

	app_ctx.set_steam_path(steam_path);

	option long_options[] = {
		{ "list",		no_argument,		nullptr, 'l' },
		{ "version",	no_argument,		nullptr, 'v' },
		{ "steam-path", required_argument,	nullptr, 'p' },
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
				const auto account_names = app_ctx.get_all_account_names();
				for (auto &account_name : account_names)
				{
					std::cout << account_name << "\n";
				}
				return 0;
			}

			case 'v':
				std::cout << APPNAME << " [" << argv[0] << "] " << VERSION_STR << "\n";
				return 0;

			case 'p':
				app_ctx.set_steam_path(optarg);
				std::cout << "Using alternative path: " << optarg << "\n";
				break;

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
	if (app_ctx.get_most_recent() != account_name)
	{
		if (!app_ctx.set_logged_in_steam_account(account_name))
		{
			std::cout << "No account name matches '" << argv[optind] << "'\n";
			return 1;
		}

		if (!app_ctx.restart_steam())
		{
			std::cout << "Couldn't restart steam\n";
			return 1;
		}
	}

	return 0;
}