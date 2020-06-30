#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "Triplet.hpp"

// ������� ��������� int �� ������� char (����) 
int intFromBytes(std::istream& is);

// ������� �������� int � ������ char (����)
void intToBytes(std::ostream& os, int value);

// ������� ������ ��������� �����s
void readFileUncompressed(std::string& byteString, std::filesystem::path& pathToFile);

// ������� �������� ������� �����
void createFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& pathToFile);

// ������� ������ ������� �����
void readFileCompressed(std::vector<Triplet>& encoded, std::filesystem::path& pathToFile);

// ������� �������� ��������� �����
void createFileUncompressed(std::string& byteString, std::filesystem::path& pathToFile);

// �������� ������ ����� 
size_t getFileSize(std::filesystem::path& pathToFile);

