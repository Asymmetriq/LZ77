#include "tests.hpp"

void test()
{
	// “естирование правильности упаковки и распаковки
	// “ест считаетс€ пройденным, если размер распакованного файла равен размеру исходного
	std::filesystem::path rootPath = std::filesystem::current_path();
	std::filesystem::create_directory(rootPath / "Packed");
	std::filesystem::create_directory(rootPath / "Unpacked");

	std::filesystem::path testPath = rootPath / "Tests";
	std::filesystem::path packedPath = rootPath / "Packed";
	std::filesystem::path unpackedPath = rootPath / "Unpacked";


	int nameIter = 1;
	std::string testname;

	uint64_t size;
	std::vector<uint64_t> sizesRaw;
	std::vector<uint64_t> sizesUnpacked;

	LZ77 result(10, 8);


	for (auto& file : std::filesystem::directory_iterator(testPath))
	{
		std::filesystem::path pathToFile = file.path();

		size = getFileSize(pathToFile);
		result.pack(pathToFile, packedPath);
		testname = "packed" + std::to_string(nameIter) + ".lz77";
		std::filesystem::rename(packedPath / "packed.lz77", packedPath / testname);
		sizesRaw.push_back(size);
		nameIter++;
	}

	nameIter = 1;

	for (auto& file : std::filesystem::directory_iterator(packedPath))
	{
		std::filesystem::path pathToFile = file.path();

		size = getFileSize(pathToFile);
		result.unpack(pathToFile, unpackedPath);
		testname = "unpacked" + std::to_string(nameIter) + ".unlz77";
		std::filesystem::rename(unpackedPath / "unpacked.unlz77", unpackedPath / testname);
		sizesUnpacked.push_back(size);
		nameIter++;
	}

	for (size_t i = 0; i < sizesRaw.size(); i++)
		if (sizesRaw[i] = sizesUnpacked[i])
			std::cout << "Equal sizes for test " + std::to_string(i + 1) + ". Passed." << std::endl;
}