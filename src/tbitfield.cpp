#include "tbitfield.h"

TBitField::TBitField(int len) // конструктор
{
	if (len <= 0) // Исключение
		throw std::invalid_argument("Length must be positive");

	BitLen = len;
	MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8); // округляем вверх, берем ближайшее не меньшее требуемого числа бит, кратное 32
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	// Копируем данные
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() // Деструктор
{
	delete[] pMem; // Очищаем память
	pMem = nullptr; // Избавляемся от "висячего указателя"
}

int TBitField::GetMemIndex(const int n) const
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // Получение длины поля
{
	return BitLen;
}

void TBitField::SetBit(const int n)
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("Bit index out of range");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask;
}

void TBitField::ClrBit(const int n)
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("Bit index out of range");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] &= ~mask; // сбрасываем побитовым умножением на инверсию
}

int TBitField::GetBit(const int n) const
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("Bit index out of range");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf)
{
	if (this == &bf)
		return *this;

	TELEM* temppMem = new TELEM[bf.MemLen];
	// Тут не нужно исключение, new сам его кинет, если кончится память

	for (int i = 0; i < bf.MemLen; i++)
		temppMem[i] = bf.pMem[i];

	delete[] pMem;
	BitLen = bf.BitLen;
	pMem = temppMem;
	MemLen = bf.MemLen;
	return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
	if (BitLen != bf.BitLen) // Проверяем равенство длин
		return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0; // Проверяем байты на неравенство

	return 1; // Возвращаем 1, если все хорошо
}

int TBitField::operator!=(const TBitField& bf) const
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf)
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen); // Находим максимальное по длине боле

	int minMemLen = (MemLen > bf.MemLen) ? bf.MemLen : MemLen; // Находим минимальную по длине память

	for (int i = 0; i < minMemLen; i++) // Заполняем минимальную часть
		result.pMem[i] = pMem[i] | bf.pMem[i];
	// Копируем хвост
	if (MemLen > bf.MemLen)
		for (int i = minMemLen; i < MemLen; i++)
			result.pMem[i] = pMem[i];
	else if (bf.MemLen > MemLen)
		for (int i = minMemLen; i < bf.MemLen; i++)
			result.pMem[i] = bf.pMem[i];

	return result; // Возвращаем объект
}

TBitField TBitField::operator&(const TBitField& bf)
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen); // Находим максимальное по длине боле

	int minMemLen = (MemLen > bf.MemLen) ? bf.MemLen : MemLen; // Находим минимальную по длине память

	for (int i = 0; i < minMemLen; i++) // Заполняем общую часть
		result.pMem[i] = pMem[i] & bf.pMem[i];

	return result; // Возвращаем объект
}

TBitField TBitField::operator~(void)
{
	TBitField result(BitLen); // создаем результат, чтобы не изменять текущий объект. Иначе это небезопасно
	// Инвертируем значимую часть
	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = ~pMem[i];

	int LastSignificantBits = BitLen % (sizeof(TELEM) * 8);
	if (LastSignificantBits > 0)
	{
		TELEM mask = (1 << LastSignificantBits) - 1;
		result.pMem[MemLen - 1] &= mask;
	}

	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf)
{
	int len; // Создаем переменную для хранения длины
	istr >> len; // Получаем на вход длину битового поля
	if (istr.fail() || len <= 0)
		throw std::invalid_argument("Invalid max power");

	// Создаем временный объект
	TBitField temp(len);

	// Дальше считываем биты
	for (int i = 0; i < len; i++)
	{
		int bit; // Создаем переменную под бит
		istr >> bit; // Считываем бит
		if (bit == 1)
			temp.SetBit(i); // Устанавливаем бит, если 1
		else if (bit == 0)
			temp.ClrBit(i); // Очищаем бит, если 0
		else // Бросаем исключение, если ввели что-то кроме 0 и 1
			throw std::invalid_argument("Invalid bit value");
	}

	bf = temp; // Присваиваем целевому объекту временный объект (при этом не повредив его)

	return istr; // Возвращаем поток для цепочки ввода
}

ostream& operator<<(ostream& ostr, const TBitField& bf)
{
	ostr << "{ "; // Сначала выведем открывающую поле скобку

	// Выведем все биты поля
	for (int i = 0; i < bf.BitLen; i++)
	{
		// Выводим значение бита текущей итерации вывода
		ostr << bf.GetBit(i) << " ";

		// Каждые 32 бита будем делать перенос строки для читаемости
		if ((i + 1) % 32 == 0)
			ostr << endl << " ";
	}

	// Закрываем поле скобкой
	ostr << "}";

	return ostr; // Возвращаем поток для цепочки ввода
}
