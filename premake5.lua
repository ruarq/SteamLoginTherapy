workspace "RuarqsSteamAccountSwitcherForLinux"
	configurations { "debug", "release" }

project "steam_login"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	files { "src/**.hpp", "src/**.cpp" }

	filter { "configurations:debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:release" }
		optimize "On"
