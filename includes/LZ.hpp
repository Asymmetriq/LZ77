#pragma once
#include <iterator>
#include <algorithm>
#include "Triplet.hpp"
#include "ReadWriteTools.hpp"
#include "tests.hpp"


class LZ77
{
	struct slidingWindow
	{
		// Буфер предпросмотра и буфер истории
		std::string lookaheadBuffer;
		std::string historyBuffer;

		// Максимальные размеры буфера предпросмотра и буфера истории
		size_t lookBufferMax;
		size_t histBufferMax;

		slidingWindow() = default;
		slidingWindow(std::string hbf, std::string lhbf) : historyBuffer(hbf), lookaheadBuffer(lhbf) {}
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


	// Функция компрессии (сжатия)
	void compress();

	// Функция декомпрессии
	void decompress();

	// Функция сброса
	void reset();

public:

	// Функция упаковки
	void pack(std::filesystem::path& inpath, std::filesystem::path& outpath);

	// Функция распаковки
	void unpack(std::filesystem::path& inpath, std::filesystem::path& outpath);

	LZ77(size_t lookBufMaxSize, size_t histBufMaxSize)
	{
		window.lookBufferMax = lookBufMaxSize * 1024;
		window.histBufferMax = histBufMaxSize * 1024;
	};
};
