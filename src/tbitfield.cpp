// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len)
{
	if (len < 0)
		throw std::invalid_argument("Negative length");
	BitLen = len;
	MemLen = ((len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8));
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
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
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw std::out_of_range("Bit index is negative");
	if (n >= BitLen)
		throw std::out_of_range("Bit index is too large");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw std::out_of_range("Bit index is negative");
	if (n >= BitLen)
		throw std::out_of_range("Bit index is too large");
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)
		throw std::out_of_range("Bit index is negative");
	if (n >= BitLen)
		throw std::out_of_range("Bit index is too large");
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) && 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i])
			return 0;
	}
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 1;
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i])
			return 1;
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int MaxBit;
	if (bf.BitLen > BitLen)
		MaxBit = bf.BitLen;
	else
		MaxBit = BitLen;
	TBitField tmp(MaxBit);
	for (int i = 0; i < tmp.MemLen; i++) {
		if ((i < MemLen) && (i < bf.MemLen))
			tmp.pMem[i] = pMem[i] | bf.pMem[i];
		else if (i < MemLen)
			tmp.pMem[i] = pMem[i];
		else
			tmp.pMem[i] = bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int MaxBit;
	if (bf.BitLen > BitLen)
		MaxBit = bf.BitLen;
	else
		MaxBit = BitLen;
	TBitField tmp(MaxBit);
	int MinMem;
	if (bf.MemLen < MemLen)
		MinMem = bf.MemLen;
	else
		MinMem = MemLen;
	for (int i = 0; i < MinMem; i++)
		tmp.pMem[i] = pMem[i] & bf.pMem[i];
	return tmp;

}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int n = 0; n < BitLen; n++) {
		if (GetBit(n) == 0)
			tmp.SetBit(n);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char bit;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> bit;
		if (bit == '0')
			bf.ClrBit(i);
		else if (bit == '1')
			bf.SetBit(i);
		else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
