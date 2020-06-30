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
		// ����� ������������� � ����� �������
		std::string lookaheadBuffer;
		std::string historyBuffer;

		// ������������ ������� ������ ������������� � ������ �������
		size_t lookBufferMax;
		size_t histBufferMax;

		slidingWindow() = default;
		slidingWindow(std::string hbf, std::string lhbf) : historyBuffer(hbf), lookaheadBuffer(lhbf) {}
		~slidingWindow() = default;

		// ������� ��������� ������ �������� ������������ ��������
		Triplet getLongestPrefix();
	};

	// ��������� ����������� ���� (�������� �� 2 ������)
	LZ77::slidingWindow window;

	// ������, �������� ������������� ������ ����� char
	std::string byteDataString;

	// ������ �����-����� <offset, length, next>
	std::vector<Triplet> encoded;


	// ������� ���������� (������)
	void compress();

	// ������� ������������
	void decompress();

	// ������� ������
	void reset();

public:

	// ������� ��������
	void pack(std::filesystem::path& inpath, std::filesystem::path& outpath);

	// ������� ����������
	void unpack(std::filesystem::path& inpath, std::filesystem::path& outpath);

	LZ77(size_t lookBufMaxSize, size_t histBufMaxSize)
	{
		window.lookBufferMax = lookBufMaxSize * 1024;
		window.histBufferMax = histBufMaxSize * 1024;
	};
};
