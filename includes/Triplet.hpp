#pragma once

// Структура кодов-троек для записи и чтения результатов сжатия
struct Triplet
{
	int offset;
	int length;
	char next;

	Triplet() = default;
	Triplet(int off, int len, char next) : offset(off), length(len), next(next) {};
	~Triplet() = default;
};

