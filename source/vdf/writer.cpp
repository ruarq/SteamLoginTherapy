#include "writer.hpp"

namespace vdf
{

namespace
{

int indent = 0;

}

auto write_indent(std::ostream &out)
{
	for (int i = 0; i < indent; ++i)
	{
		out << "\t";
	}
}

auto write_value(std::ostream &out, const vdf::value *value) -> void;
auto write_list(std::ostream &out, const vdf::list *list) -> void;

auto write_object(std::ostream &out, const vdf::object *object) -> void
{
	if (!object)
	{
		DEBUG_LOG("object is nullptr");
		return;
	}

	write_indent(out);
	out << '"' << object->key << '"';

	if (auto value = dynamic_cast<const vdf::value*>(object))
	{
		write_value(out, value);
	}
	else if (auto list = dynamic_cast<const vdf::list*>(object))
	{
		write_list(out, list);
	}
	else
	{
		DEBUG_LOG("object is not a list or a value");
	}
}

auto write_value(std::ostream &out, const value *value) -> void
{
	out << "\t\t\"" << value->val << "\"\n";
}

auto write_list(std::ostream &out, const list *list) -> void
{
	out << "\n";
	write_indent(out);
	out << "{\n";
	++indent;
	for (auto object : *list)
	{
		write_object(out, object);
	}
	out << "\n";
	--indent;
	write_indent(out);
	out << "}\n";
}

auto write_file(const std::string &filename, const object *vdf_tree) -> bool
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		return false;
	}

	write_object(file, vdf_tree->at(0));
	return true;
}

}