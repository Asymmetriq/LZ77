#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "Triplet.hpp"

// Функция получения int из массива char (байт) 
int intFromBytes(std::istream& is);

// Функция перевода int в массив char (байт)
void intToBytes(std::ostream& os, int value);

// Функция чтения несжатого файлаs
void readFileUncompressed(std::string& byteString, std::filesystem::path& pathToFile);

// Функция создания сжатого файла
void createFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& pathToFile);

// Функция чтения сжатого файла
void readFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& pathToFile);

// Функция создания несжатого файла
void createFileUncompressed(std::string& byteString, std::filesystem::path& pathToFile);

// Получаем размер файла 
size_t getFileSize(std::filesystem::path& pathToFile);

