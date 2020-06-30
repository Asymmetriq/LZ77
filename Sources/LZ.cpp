#include "LZ.hpp"

// Получаем самый длинный совпадающий префикс
Triplet LZ77::slidingWindow::getLongestPrefix()
{
	// Наименьший код-тройка 
	Triplet code(0, 0, lookaheadBuffer[0]);

	size_t lookCurrLen = lookaheadBuffer.length() - 1;
	size_t histCurrLen = historyBuffer.length();

	// Просматриваем все подстроки в буфере предпросмотра
	for (size_t i = 1; i <= std::min(lookCurrLen, histCurrLen); i++)
	{
		// Формируем подстроку нужной длины
		std::string s = lookaheadBuffer.substr(0, i);

		size_t pos = historyBuffer.find(s);
		if (pos == std::string::npos)
			break;

		// Проверяем, не сформировалась ли на предыдущем шаге текущая подстрока + могут ли быть повторы 
		if ((historyBuffer.compare(histCurrLen - i, i, s) == 0) 
		&& (lookaheadBuffer[0] == lookaheadBuffer[i]))
			pos = histCurrLen - i;

		// Если нашли подстроку в буфере истории, то смотрим, есть ли следом в буфере предпросмотра её повторения
		size_t fullRepeat = 0;
		if (histCurrLen == pos + i)
		{
			// Проверяем, есть ли в буфере предпросмотра полные повторения текущей подстроки (следом за текущей)
			while ((lookCurrLen >= i + fullRepeat + i) 
			&& (lookaheadBuffer.compare(i + fullRepeat, i, s) == 0))
				fullRepeat += i;

			// Проверяем, есть ли в буфере предпросмотра частичные повторения текущей подстроки (следом за текущей)
			size_t partRepeat = i - 1;
			while (!((lookCurrLen >= i + fullRepeat + partRepeat)
			&& (lookaheadBuffer.compare(i + fullRepeat, partRepeat, s, 0, partRepeat) == 0)) && partRepeat)
				partRepeat--;

			fullRepeat += partRepeat;
		}

		// Cравниваем длины предыдущего максимального узла и текущего
		if (code.length <= i + fullRepeat)
			code = Triplet(histCurrLen - pos, i + fullRepeat, lookaheadBuffer[i + fullRepeat]);
	}
	return code;
}

// Компрессинг
void LZ77::compress()
{
	do
	{
		// Добавляем символы в освободившуюся часть буфера предпросмотра
		if ((window.lookaheadBuffer.length() < window.lookBufferMax) && (byteDataString.length() != 0))
		{
			int len = window.lookBufferMax - window.lookaheadBuffer.length();
			window.lookaheadBuffer.append(byteDataString, 0, len);
			byteDataString.erase(0, len);
		}

		Triplet triplet = window.getLongestPrefix();

		// Добавляем в буфер истории отработанную часть буфера предпросмотра
		window.historyBuffer.append(window.lookaheadBuffer, 0, triplet.length + 1);
		window.lookaheadBuffer.erase(0, triplet.length + 1); // Удаляем эту часть из окна предпросмотра

		// Если длина буфера больше максимальной, удаялем самые старые символы
		if (window.historyBuffer.length() > window.histBufferMax)
			window.historyBuffer.erase(0, window.historyBuffer.length() - window.histBufferMax);

		encoded.push_back(triplet);

	} while (window.lookaheadBuffer.length());
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

// Функция сброса
void LZ77::reset()
{
	encoded.clear();
	window.historyBuffer.clear();
	window.lookaheadBuffer.clear();
	byteDataString.clear();
}

// Функция упаковки
void LZ77::pack(std::filesystem::path& inPath, std::filesystem::path& outPath)
{
	readFileUncompressed(byteDataString, inPath);
	compress();
	createFileCompressed(encoded, outPath);
	reset();
}

// Функция распаковки
void LZ77::unpack(std::filesystem::path& inPath, std::filesystem::path& outPath)
{
	readFileCompressed(encoded, inPath);
	decompress();
	createFileUncompressed(byteDataString, outPath);
	reset();
}