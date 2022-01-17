#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include "../Log.hpp"
#include "parser.hpp"
#include "object.hpp"

namespace vdf
{

auto read_file(const std::string &filename) -> object*;

}