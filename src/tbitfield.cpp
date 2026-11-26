// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <string>


TBitField::TBitField(int len)
{
	if (len < 0) {
		throw std::invalid_argument("");
	}
	BitLen = len;
	int k = sizeof(TELEM) * 8; //кол-во битов в одном элементе массива
	MemLen = (len + k - 1) / k;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (MemLen > 0) {
		pMem = new TELEM[MemLen];
	}
	else {
		pMem = nullptr;
	}
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
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
	int k = sizeof(TELEM) * 8;
	return (TELEM)1 << (n % k);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("");
	}
	pMem[GetMemIndex(n)] |= (GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("");
	}
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("");
	}
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this == &bf) {
		return *this;
	}
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (MemLen > 0) {
		pMem = new TELEM[MemLen];
	}
	else {
		pMem = nullptr;
	}
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf) {
		return 0;
	}
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int biglen;
	if (bf.BitLen > BitLen) {
		biglen = bf.BitLen;
	}
	else
	{
		biglen = BitLen;
	}
	TBitField result(biglen);
	for (int i = 0; i < result.MemLen; i++) {
		TELEM a, b;
		if (i < MemLen) {
			a = pMem[i];
		}
		else
		{
			a = 0;
		}
		if (i < bf.MemLen) {
			b = bf.pMem[i];
		}
		else
		{
			b = 0;
		}
		result.pMem[i] = a | b;
	}
	return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int biglen;
	if (bf.BitLen > BitLen) {
		biglen = bf.BitLen;
	}
	else
	{
		biglen = BitLen;
	}
	TBitField result(biglen);
	for (int i = 0; i < result.MemLen; i++) {
		TELEM a, b;
		if (i < MemLen) {
			a = pMem[i];
		}
		else
		{
			a = 0;
		}
		if (i < bf.MemLen) {
			b = bf.pMem[i];
		}
		else
		{
			b = 0;
		}
		result.pMem[i] = a & b;
	}
	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++) {
		result.pMem[i] = ~pMem[i];
	}
	int k = (int)(sizeof(TELEM) * 8); //кол-во битов в одном элементе массива
	int remain = BitLen % k;
	if (remain != 0 && MemLen > 0) {
		result.pMem[MemLen - 1] &= ((TELEM)1 << remain) - 1;
	}
	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	std::string str;
	istr >> str;
	if ((int)str.size() != bf.BitLen) {
		throw std::invalid_argument("");
	}
	for (int i = 0; i < bf.BitLen; i++) {
		if (str[i] == '1') {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}