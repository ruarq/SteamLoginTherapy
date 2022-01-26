#pragma once

#include <cstdint>

#define APPNAME "Steam Login Therapy"
#define _VERSION_MACRO(major, minor, patch) \
	static constexpr auto VERSION_MAJOR = major; \
	static constexpr auto VERSION_MINOR = minor; \
	static constexpr auto VERSION_PATCH = patch; \
	static constexpr auto VERSION_STR = #major "." #minor "." #patch; \
	static constexpr std::int64_t VERSION = ((std::int64_t)(major) << 32) + (minor << 16) + patch

_VERSION_MACRO(0, 1, 1);