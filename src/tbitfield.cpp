// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <stdexcept>
#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len >= 0) {
		BitLen = len;
		MemLen = (BitLen + SOffT - 1) / SOffT;
		pMem = new TELEM[MemLen]();
	}
	else
		throw std::invalid_argument("len < 0");
}

TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen)// конструктор копирования
{
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("bit index out of range");
	return (n) / SOffT;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("bit index out of range");
	return (TELEM)1 << (n % SOffT);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("bit index out of range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("ClrBit - bit index out of range");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("GetBit - bit index out of range");
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (MemLen != bf.MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return (*this == bf) ? 0 : 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
	TBitField res(maxLen);

	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = pMem[i] | bf.pMem[i];

	if (MemLen > bf.MemLen)
		for (int i = minMemLen; i < MemLen; i++)
			res.pMem[i] = pMem[i];
	else
		for (int i = minMemLen; i < bf.MemLen; i++)
			res.pMem[i] = bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
	TBitField res(maxLen);
	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	if (BitLen == 0) return res;
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}
	if (BitLen % SOffT != 0) {
		res.pMem[MemLen - 1] = res.pMem[MemLen - 1] & (TELEM)(((TELEM)1 << (BitLen - SOffT * (MemLen - 1))) - 1);
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	for (int i = bf.BitLen - 1; i >= 0; i--) {
		char c;
		istr >> c;
		if (!istr)
			break;
		if (c == '1')
			bf.SetBit(i);
		else if (c == '0')
			bf.ClrBit(i);
		else {
			istr.setstate(std::ios::failbit);
			break;
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = bf.BitLen - 1; i >= 0; i--)
		ostr << (bf.GetBit(i) ? '1' : '0');
	return ostr;
}
