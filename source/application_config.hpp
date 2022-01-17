#pragma once

#include <cstdint>

#define APPNAME "Steam Login Therapy"
#define __VERSION_MACRO(major, minor, patch) \
	static constexpr auto VERSION_MAJOR = major; \
	static constexpr auto VERSION_MINOR = minor; \
	static constexpr auto VERSION_PATCH = patch; \
	static constexpr auto VERSION_STR = #major "." #minor "." #patch; \
	static constexpr std::int32_t VERSION = (major << 16) + (minor << 8) + patch

__VERSION_MACRO(0, 0, 0);