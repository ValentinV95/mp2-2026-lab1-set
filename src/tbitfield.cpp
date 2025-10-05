// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw new std::invalid_argument("Invalid length!");
	else {
		BitLen = len;
		MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) throw std::invalid_argument("Invalid index");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) throw std::invalid_argument("Invalid index");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) throw std::invalid_argument("Invalid index");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (this != &bf) {
		if (BitLen != bf.BitLen) return 0;
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 0;
		}
		return 1;
	}
	else return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (this != &bf) {
		if (BitLen != bf.BitLen) return 1;
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 1;
		}
		return 0;
	}
	else return 0;
}

TBitField TBitField::operator|(const TBitField& bf) {
	int maxLen = max(BitLen, bf.BitLen);
	TBitField result(maxLen);
	int minMemLen = min(MemLen, bf.MemLen);

	for (int i = 0; i < minMemLen; i++) {
		int iPmem = (pMem[i] | bf.pMem[i]);
		result.pMem[i] = iPmem;
		std::cout << result.pMem[i];
	}

	if (MemLen > bf.MemLen) {
		for (int i = bf.MemLen; i < MemLen; i++) {
			result.pMem[i] = pMem[i];
		}
	}
	else {
		for (int i = MemLen; i < bf.MemLen; i++) {
			result.pMem[i] = bf.pMem[i];
		}
	}

	return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
	int minLen = min(BitLen, bf.BitLen);
	TBitField result(minLen);

	int minMemLen = min(MemLen, bf.MemLen);
	for (int i = 0; i < minMemLen; i++) {
		result.pMem[i] = (pMem[i] & bf.pMem[i]);
	}

	return result;
}

TBitField TBitField::operator~(void) {
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++) {
		result.pMem[i] = ~pMem[i];
	}

	int extraBits = BitLen % (sizeof(TELEM) * 8);
	if (extraBits != 0) {
		TELEM mask = (1 << extraBits) - 1;
		result.pMem[MemLen - 1] &= mask;
	}

	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	char ch;
	int bitCounter = 0;
	for (int i = 0; i < bf.MemLen; i++)
		bf.pMem[i] = 0;
	while (bitCounter < bf.BitLen && istr.get(ch))
	{
		if (ch == '1')
		{
			bf.SetBit(bitCounter);
			bitCounter++;
		}
		else if (ch == '0')
		{
			bf.ClrBit(bitCounter);
			bitCounter++;
		}
		else if (ch == ' ' || ch == '\t' || ch == '\n')
		{
			continue;
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << (bf.GetBit(i) ? '1' : '0');
	}
	return ostr;
}
