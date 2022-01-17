#include "value.hpp"

namespace vdf
{

value::value(const std::string &value)
	: val(value)
{
}

value::value(const std::string &key, const std::string &value)
	: val(value)
{
	this->key = key;
}

}