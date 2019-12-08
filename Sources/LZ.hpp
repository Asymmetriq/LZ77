#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>


class LZ77
{
	struct Triplet
	{
		int offset;
		int length;
		char next;

		Triplet() = default;
		Triplet(int off, int len, char next) : offset(off), length(len), next(next) {};
		~Triplet() = default;
	};

	struct slidingWindow
	{
		// Буфер предпросмотра и буфер истории
		std::string lookheadBuffer;
		std::string historyBuffer;
		
		// Максимальные размеры буфера предпросмотра и буфера истории
		size_t lookBufferMax;
		size_t histBufferMax;
		
		slidingWindow() = default;
		slidingWindow(std::string hbf, std::string lhbf) : historyBuffer(hbf), lookheadBuffer(lhbf) {}
		~slidingWindow() = default;

		// Функция получения самого длинного совпадающего префикса
		Triplet getLongestPrefix();
	};

	// Структура скользящего окна (поделено на 2 буфера)
	LZ77::slidingWindow window;

	// Строка, хранящая интерпретацию байтов через char
	std::string byteDataString;

	// Вектор кодов-троек <offset, length, next>
	std::vector<Triplet> encoded;

public:

	// Функция компрессии (сжатия)
	void compress();

	// Функция декомпрессии
	void decompress();

	// Функция чтения несжатого файла
	void readFileUncompressed(std::filesystem::path& pathToFile);

	// Функция создания сжатого файла
	void createFileCompressed(std::filesystem::path& pathToFile);

	// Функция чтения сжатого файла
	void readFileCompressed(std::filesystem::path& pathToFile);

	// Функция создания несжатого файла
	void createFileUncompressed(std::filesystem::path& pathToFile);

	// Функция сброса
	void reset()
	{
		encoded.clear();
		window.historyBuffer.clear();
		window.lookheadBuffer.clear();
		byteDataString.clear();
	}

	LZ77(size_t lookBufMaxSize, size_t histBufMaxSize)
	{
		window.lookBufferMax = lookBufMaxSize * 1024;
		window.histBufferMax = histBufMaxSize * 1024;
	};
};
