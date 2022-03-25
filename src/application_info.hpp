#pragma once

#include <cstdint>

#define APPNAME "Steam Login Therapy"
#define _VERSION_MACRO(major, minor, patch) \
	static constexpr auto VERSION_MAJOR = major; \
	static constexpr auto VERSION_MINOR = minor; \
	static constexpr auto VERSION_PATCH = patch; \
	static constexpr auto VERSION_STR = #major "." #minor "." #patch; \
	static constexpr std::int64_t VERSION = ((std::int64_t)(major) << 32) + (minor << 16) + patch

_VERSION_MACRO(0, 2, 0);

#if defined(__linux__)
#define SLT_OS_LINUX
#elif defined(__APPLE__)
#define SLT_OS_MACOSX
#else
#error "Your operating system is not supported."
#endif
