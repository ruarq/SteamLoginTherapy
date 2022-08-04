workspace "SteamLoginTherapy"
	configurations { "debug", "release" }

project "SteamLogin"
	kind "ConsoleApp"
	targetname "steam-login"
	language "C++"
	cppdialect "C++20"

	buildoptions { "`pkg-config fmt --cflags`" }
	linkoptions { "`pkg-config fmt --libs`" }

	files { "src/**.hpp", "src/**.cpp" }

	filter { "configurations:debug" }
		symbols "On"
		defines { "DEBUG" }

	filter { "configurations:release" }
		optimize "On"
