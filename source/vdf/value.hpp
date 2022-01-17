#pragma once

#include <string>

#include "object.hpp"

namespace vdf
{

class value final : public object
{
public:
	value(const std::string &value);
	value(const std::string &key, const std::string &value);

public:
	std::string val;
};

}