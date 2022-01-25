#include "parser.hpp"

namespace vdf
{

auto parser::parse(const std::string &vdf_string) -> object*
{
	source = vdf_string;
	pos = 0;

	if (vdf_string.empty())
	{
		DEBUG_LOG("vdf_string is empty");
		return nullptr;
	}

	auto root = new list({ parse_object() });
	root->key = "";
	
	if (!error)
	{
		return root;
	}
	else
	{
		delete root;
		return nullptr;
	}
}

auto parser::had_error() const -> bool
{
	return error;
}

auto parser::current() const -> char
{
	if (!eof())
	{
		return source[pos];
	}
	else
	{
		return 0;
	}
}

auto parser::consume() -> char
{
	auto result = current();
	++pos;
	return result;
}

auto parser::skip_whitespace() -> void
{
	while (std::isspace(current()))
	{
		consume();
	}
}

auto parser::eof() const -> bool
{
	return pos >= source.size();
}

auto parser::parse_object() -> object*
{
	skip_whitespace();

	if (current() != '"')
	{
		DEBUG_LOG("expected \" but found " << current());
		error = true;
		return nullptr;
	}

	std::string key;
	consume();
	while (current() != '"' && !eof())
	{
		key += consume();
	}
	consume();

	object *result = nullptr;

	skip_whitespace();
	if (current() == '"')
	{
		result = parse_value();
	}
	else if (current() == '{')
	{
		result = parse_list();
	}

	if (result)
	{
		result->key = key;
	}

	return result;
}

auto parser::parse_value() -> value*
{
	skip_whitespace();

	if (current() != '"')
	{
		DEBUG_LOG("expected \" but found " << current());
		error = true;
		return nullptr;
	}

	std::string value;
	consume();
	while (current() != '"' && !eof())
	{
		value += consume();
	}
	consume();

	return new vdf::value(value);
}

auto parser::parse_list() -> list*
{
	skip_whitespace();

	if (current() != '{')
	{
		DEBUG_LOG("expected { but found " << current());
		error = true;
		return nullptr;
	}

	std::vector<object*> objects;
	consume();
	while (current() != '}' && !eof())
	{
		objects.push_back(parse_object());
		skip_whitespace();
	}
	consume();

	return new list(objects);
}

}