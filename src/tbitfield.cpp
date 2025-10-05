// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) { throw std::invalid_argument("Bit lenth must be positive"); }

	BitLen = len;
	int BitsOfElement = sizeof(TELEM) * 8; //32 или 64
	MemLen = (BitsOfElement + (BitLen - 1)) / BitsOfElement; //округление вверх, сколько элементов выделяем
	pMem = new TELEM[MemLen]; //создаём массив для элементов unsigned int
	for (int i = 0; i < MemLen; i++) { pMem[i] = 0; } //обнуляем этот массив
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) { pMem[i] = bf.pMem[i]; }
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) { throw std::out_of_range("The bit index is wrong"); }

	int BitsOfElement = sizeof(TELEM) * 8; //32 или 64
	int MemIndex = n / BitsOfElement; //округляем вниз, т.к. нумерация в pMem с индекса [0]
	return MemIndex;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int N = n % (sizeof(TELEM) * 8); //место бита в конкретном элементе pMem
	TELEM MaskOfBit = 1u << N; //сдвигаем 1 на место N
	return MaskOfBit;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen; //возвращаем длину битового поля
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) { throw std::out_of_range("The bit index is wrong"); }

	int index = GetMemIndex(n);   // номер в pMem
	TELEM mask = GetMemMask(n);   // маска для этого бита
	pMem[index] = pMem[index] | mask; //поставили в этот бит 1
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) { throw std::out_of_range("The bit index is wrong"); }

	int index = GetMemIndex(n);   // номер в pMem
	TELEM mask = ~GetMemMask(n);   // маска для этого бита, где вместо 1 в маске 0, остальные - единицы
	pMem[index] = pMem[index] & mask; //поставили в этот бит 0
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) { throw std::out_of_range("The bit index is wrong"); }
	int index = GetMemIndex(n);   // номер в pMem
	TELEM mask = GetMemMask(n);   // маска для этого бита
	if (pMem[index] & mask){ return 1; } //& даст число != 0, значит на конкретном бите стояла 1
	else{ return 0; } //& даст 0, значит этот быит = 0
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) { //если размеры pMem разные
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++) { //переносим правый элемент в левый
		pMem[i] = bf.pMem[i];
	}
	return *this; //возвращаем левый элемент по ссылке
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) { return false; } //количество значимых битов разное
	if (MemLen == 0) { return 1; } //у обоих уже одинаковое BitLen, значит, если у одного MemLen = 0,то и у другого тоже
	for (int i = 0; i < MemLen - 1; i++) //проверяем полные элементы pMem
	{ 
		if (pMem[i] != bf.pMem[i]) { return false; }
	}
	int k = BitLen % (sizeof(TELEM) * 8); //сколько битов осталось в последнем неполном элементе
	if (k == 0) //последний элемент заполнен полностью
	{ 
		if (pMem[MemLen - 1] != bf.pMem[MemLen - 1]) return 0;
	}
	else 
	{ //последний элемент не полностью заполнен
		TELEM mask = (1u << k) - 1; //k младших битов обращаются в 1
		if ((pMem[MemLen - 1] & mask) != (bf.pMem[MemLen - 1] & mask)) return 0; //обнуляем ненужные и сравниваем то, что осталось
	}
	return true; //не попали ни в одно из условий, в остальных случаях будут равны
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int LenOfRes = 0;
	if (BitLen > bf.BitLen) { LenOfRes = BitLen; } //выбираем наибольшую длину
	else { LenOfRes = bf.BitLen; }
	TBitField result(LenOfRes);
	for (int i = 0; i < result.MemLen; i++) 
	{
		TELEM left = 0;
		TELEM right = 0;
		if (i < MemLen) { left = pMem[i]; }
		if (i < bf.MemLen) { right = bf.pMem[i]; }
		result.pMem[i] = left | right;
	}
	int k = result.BitLen % (sizeof(TELEM) * 8);
	if (k != 0) //оставляем младшие биты
	{
		TELEM mask = (TELEM(1) << k) - 1;
		result.pMem[result.MemLen - 1] &= mask; 
	}
	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int LenOfRes;
	if (BitLen > bf.BitLen){ LenOfRes = BitLen; }//выбираем большую длину
	else { LenOfRes = bf.BitLen; }
	TBitField result(LenOfRes);
	for (int i = 0; i < result.MemLen; i++) 
	{
		TELEM left = 0;
		TELEM right = 0;
		if (i < MemLen){ left = pMem[i]; }
		if (i < bf.MemLen){ right = bf.pMem[i]; }
		result.pMem[i] = left & right;
	}
	int k = result.BitLen % (sizeof(TELEM) * 8);
	if (k != 0) //обнуляем лишние старшие биты
	{
		TELEM mask = (1u << k) - 1;
		result.pMem[result.MemLen - 1] &= mask; 
	}
	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++)//инвертируем каждый элемент pMem по отдельности
	{
		result.pMem[i] = ~pMem[i]; 
	}
	int k = BitLen % (sizeof(TELEM) * 8);
	if (k != 0) //оставляем только младшие биты
	{
		TELEM mask = (TELEM(1) << k) - 1; 
		result.pMem[MemLen - 1] &= mask;
	}
	return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string s;
	istr >> s; // читаем строку вида "10101"
	for (int i = 0; i < bf.BitLen; i++) //обнуляем биты
	{ 
		bf.ClrBit(i); 
	}
	int len = s.length();
	for (int i = 0; (i < len) && (i < bf.BitLen); i++)  //идем по строке слева направо, заполняем наоборот
	{
		if (s[len - i - 1] == '1')
			bf.SetBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = bf.BitLen - 1; i >= 0; i--) 
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
