#include "LZ.hpp"


int main(int argc, char* argv[])
{
	std::filesystem::path iPath(argv[1]);
	std::filesystem::path oPath(argv[2]);
	std::string argument = argv[3];

	LZ77 result(10, 8);

	// Упаковка
	if (argument == "p")
		result.pack(iPath, oPath);

	// Распаковка
	else if (argument == "u")
		result.unpack(iPath, oPath);

	else if (argument == "t")
		test();

	else
		std::cout << "Wrong cmd argument" << std::endl;
}
	