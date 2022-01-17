#pragma once

#include <iostream>

#if defined(DEBUG)
#define DEBUG_LOG(x) std::cout << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ": " << x << "\n";
#else
#define DEBUG_LOG(x)
#endif