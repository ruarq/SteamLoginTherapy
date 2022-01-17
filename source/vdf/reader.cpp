#include "reader.hpp"

namespace vdf
{

auto read_file(const std::string &filename) -> object*
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		DEBUG_LOG("Couldn't open '" << filename << "'");
		return nullptr;
	}

	std::string source, line;
	while (std::getline(file, line))
	{
		source += line + "\n";
	}

	return parser().parse(source);
}

}
