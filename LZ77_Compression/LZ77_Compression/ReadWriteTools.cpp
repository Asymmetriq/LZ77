#include "ReadWriteTools.hpp"

// Получаем int из массива char (байт) 
int intFromBytes(std::istream& is)
{
	char bytes[4];
	for (int i = 0; i < 4; ++i)
		is.get(bytes[i]);

	int integer;
	std::memcpy(&integer, &bytes, 4);
	return integer;
}

// Переводим int в массив char (байт)
void intToBytes(std::ostream& os, int value)
{
	char bytes[4];
	std::memcpy(&bytes, &value, 4);
	os.write(bytes, 4);
}

// Читаем несжатый файл
void readFileUncompressed(std::string& byteString, std::filesystem::path& path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		// Конструируем строку из потока с помощью итератора на начало файла и eof
		byteString = std::string(std::istreambuf_iterator<char>(file), {});
		file.close();
	}
	else
		throw std::ios_base::failure("Failed to open the file");
}

// Создаём сжатый файл
void createFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& path)
{
	std::ofstream out(path / "packed.lz77", std::ios::out | std::ios::binary);

	if (out.is_open())
	{
		for (auto triplet : encoded)
		{
			intToBytes(out, triplet.offset);
			out << triplet.next;
			intToBytes(out, triplet.length);
		}
		out.close();
	}
	else
		throw std::ios_base::failure("Failed to open the file");
}

// Читаем сжатый файл
void readFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		// Читаем код
		Triplet element;

		while (file.peek() != std::ifstream::traits_type::eof())
		{
			element.offset = intFromBytes(file);
			file.get(element.next);
			element.length = intFromBytes(file);
			encoded.push_back(element);
		}
		file.close();
	}
	else
		throw std::ios_base::failure("Failed to open the file");
}

// Создаём несжатый файл
void createFileUncompressed(std::string& byteString, std::filesystem::path& path)
{
	std::ofstream out(path / "unpacked.unlz77", std::ios::out | std::ios::binary);
	out << byteString;
	out.close();
}

// Получаем размер файла
size_t getFileSize(std::filesystem::path& pathToFile)
{
	std::ifstream file(pathToFile, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		long long size = file.tellg();
		file.close();
		return size;
	}
	else
		throw std::ios_base::failure("Failed to open the file");
}