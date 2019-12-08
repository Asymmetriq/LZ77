#include "LZ.hpp"

int main(int argc, char* argv[])
{
	std::filesystem::path iPath(argv[1]);
	std::filesystem::path oPath(argv[2]);
	std::string argument = argv[3];

	LZ77 result(10, 8);
	
	std::ifstream file(iPath, std::ios::in | std::ios::binary);

	if (argument == "pack")
	{
		// Сжатие
		result.readFileUncompressed(iPath);
		result.compress();
		result.createFileCompressed(oPath);
		result.reset();
	}
	else if (argument == "unpack")
	{
		// Распаковка
		result.readFileCompressed(iPath);
		result.decompress();
		result.createFileUncompressed(oPath);
		result.reset();
	}
	else
		std::cout << "Wrong cmd argument" << std::endl;

	file.close();

	/*std::ifstream f("test4.bmp", std::ios::in | std::ios::binary);
	f.seekg(0, std::ios::end);
	long long sizef = f.tellg();
	f.close();

	std::ifstream s(oPath / "unpacked.unlz77", std::ios::in | std::ios::binary);
	s.seekg(0, std::ios::end);
	long long sizes = s.tellg();

	std::cout << "raw " << sizef << "    " << "unpacked " << sizes << '\n';
	s.close();*/
}