workspace "RuarqsSteamAccountSwitcherForLinux"
	configurations { "debug", "release" }

project "steam_login"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir "bin/%{cfg.buildcfg}"

	files { "source/**.hpp", "source/**.cpp" }

	filter { "configurations:debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:release" }
		optimize "On"