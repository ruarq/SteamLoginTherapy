#pragma once

#include <fstream>

#include "../Log.hpp"
#include "object.hpp"
#include "value.hpp"
#include "list.hpp"

namespace vdf
{

auto write_file(const std::string &filename, const object *vdf_tree) -> bool;

}