// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>
#include <stdexcept>

#define ELEM_BITS (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	if (len <= 0) {
		throw std::invalid_argument("bitField length must be positive");
	}

	BitLen = len;
	MemLen = len / ELEM_BITS;

	if (len % ELEM_BITS != 0) {
		MemLen++;
	}
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	if (MemLen > 0) {
		pMem = new TELEM[MemLen];

		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	else {
		pMem = nullptr;
	}
}

TBitField::~TBitField()
{
	if (pMem != nullptr) {
		delete[] pMem;
	}
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / ELEM_BITS;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (TELEM)1 << (n % ELEM_BITS);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit index out of range");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit index out of range");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit index out of range");
	}
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		if (pMem != nullptr) {
			delete[] pMem;
		}

		BitLen = bf.BitLen;
		MemLen = bf.MemLen;

		if (MemLen > 0) {
			pMem = new TELEM[MemLen];

			for (int i = 0; i < MemLen; i++) {
				pMem[i] = bf.pMem[i];
			}
		}
		else {
			pMem = nullptr;
		}
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;

	if (BitLen != bf.BitLen) {
		res = 0;
	}
	else {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) {
				res = 0;
				break;
			}
		}
	}

	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;

	TBitField res(len);

	for (int i = 0; i < res.MemLen; i++) {
		TELEM a = (i < MemLen) ? pMem[i] : 0;
		TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;

		res.pMem[i] = a | b;
	}

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;

	TBitField res(len);

	for (int i = 0; i < res.MemLen; i++) {
		TELEM a = (i < MemLen) ? pMem[i] : 0;
		TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;

		res.pMem[i] = a & b;
	}

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);

	for (int i = 0; i < res.MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}

	if ((BitLen % ELEM_BITS) != 0) {
		TELEM mask = ((TELEM)1 << (BitLen % ELEM_BITS)) - 1;
		res.pMem[res.MemLen - 1] &= mask;
	}

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++) {
		char c;
		istr >> c;

		if (c == '1') {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	}

	return ostr;
}
