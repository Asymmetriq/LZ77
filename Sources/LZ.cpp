#include "LZ.hpp"

// Получаем самый длинный совпадающий префикс
LZ77::Triplet LZ77::slidingWindow::getLongestPrefix()
{
	// Наименьший код-тройка 
	Triplet code(0, 0, lookheadBuffer[0]);

	size_t lookCurrLen = lookheadBuffer.length() - 1;
	size_t histCurrLen = historyBuffer.length();

	// Просматриваем все подстроки в буфере предпросмотра
	for (size_t i = 1; i <= std::min(lookCurrLen, histCurrLen); i++)
	{
		// Формируем подстроку нужной длины
		std::string s = lookheadBuffer.substr(0, i);

		size_t pos = historyBuffer.find(s);
		if (pos == std::string::npos)
			break;

		// Проверяем, не сформировалась ли на предыдущем шаге текущая подстрока + могут ли быть повторы 
		if ((historyBuffer.compare(histCurrLen - i, i, s) == 0) && (lookheadBuffer[0] == lookheadBuffer[i]))
			pos = histCurrLen - i;

		// Если нашли подстроку в буфере истории, то смотрим, есть ли следом в буфере предпросмотра её повторения
		size_t fullRepeat = 0;
		if (histCurrLen == pos + i)
		{
			// Проверяем, есть ли в буфере предпросмотра полные повторения текущей подстроки (следом за текущей)
			while ((lookCurrLen >= i + fullRepeat + i) && (lookheadBuffer.compare(i + fullRepeat, i, s) == 0))
				fullRepeat += i;

			// Проверяем, есть ли в буфере предпросмотра частичные повторения текущей подстроки (следом за текущей)
			size_t partRepeat = i - 1;
			while (!((lookCurrLen >= i + fullRepeat + partRepeat)
			&& (lookheadBuffer.compare(i + fullRepeat, partRepeat, s, 0, partRepeat) == 0)) && partRepeat)
				partRepeat--;

			fullRepeat += partRepeat;
		}

		// Cравниваем длины предыдущего максимального узла и текущего
		if (code.length <= i + fullRepeat)
			code = Triplet(histCurrLen - pos, i + fullRepeat, lookheadBuffer[i + fullRepeat]);
	}
	return code;
}

// Компрессинг
void LZ77::compress()
{
	do
	{
		// Добавляем символы в освободившуюся часть буфера предпросмотра
		if ((window.lookheadBuffer.length() < window.lookBufferMax) && (byteDataString.length() != 0))
		{
			int len = window.lookBufferMax - window.lookheadBuffer.length();
			window.lookheadBuffer.append(byteDataString, 0, len);
			byteDataString.erase(0, len);
		}

		LZ77::Triplet triplet = window.getLongestPrefix();

		// Добавляем в буфер истории отработанную часть буфера предпросмотра
		window.historyBuffer.append(window.lookheadBuffer, 0, triplet.length + 1);
		window.lookheadBuffer.erase(0, triplet.length + 1); // Удаляем эту часть из окна предпросмотра

		// Если длина буфера больше максимальной, удаялем самые старые символы
		if (window.historyBuffer.length() > window.histBufferMax)
			window.historyBuffer.erase(0, window.historyBuffer.length() - window.histBufferMax);

		encoded.push_back(triplet);

	} while (window.lookheadBuffer.length());
}

// Декомпрессинг
void LZ77::decompress()
{
	for (auto code : encoded)
	{
		int length = code.length;
		if (length)
		{
			// Получаем рабочую подстроку
			std::string s = byteDataString.substr(byteDataString.length() - code.offset, std::min(length, code.offset));
			// Проверяем и учитываем повторения
			while (length)
			{
				int repeat = std::min(length, static_cast<int>(s.length()));
				byteDataString.append(s, 0, repeat);
				length -= repeat;
			}
		}
		byteDataString.append(1, code.next);
	}
}

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
void LZ77::readFileUncompressed(std::filesystem::path& path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		// Конструируем строку из потока с помощью итератора на начало файла и eof
		byteDataString = std::string(std::istreambuf_iterator<char>(file), {});
		file.close();
	}
	else
		throw std::ios_base::failure("Failed to open the file");
}

// Создаём сжатый файл
void LZ77::createFileCompressed(std::filesystem::path& path)
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
void LZ77::readFileCompressed(std::filesystem::path& path)
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
void LZ77::createFileUncompressed(std::filesystem::path& path)
{
	std::ofstream out(path / "unpacked.unlz77", std::ios::out | std::ios::binary);
	out << byteDataString;
	out.close();
}