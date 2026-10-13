// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) {
		throw invalid_argument("Negative length");
	}

	BitLen = len;
	
	size_t BitInElement = sizeof(TELEM) * 8;
	MemLen = (len + BitInElement - 1) / BitInElement;

	pMem = new TELEM[MemLen];

	memset(pMem, 0, MemLen * sizeof(TELEM)); // обнуление всех бит
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];

	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw invalid_argument("out of range argument");
	return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (TELEM)1 << (n%(sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || BitLen <= n) {
		throw invalid_argument("out of range argument");
	}
	size_t NumOfelement = n / (sizeof(TELEM) * 8);
	pMem[NumOfelement] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || BitLen <= n) {
		throw invalid_argument("out of range argument");
	}

	size_t NumOfelement = n / (sizeof(TELEM) * 8);
	pMem[NumOfelement] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) {
		throw invalid_argument("out of range argument");
	}

	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	TBitField tmp(bf);

	swap(pMem, tmp.pMem);
	swap(MemLen, tmp.MemLen);
	swap(BitLen, tmp.BitLen);

	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (size_t i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	for (size_t i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 1;
		}
	}return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	size_t MaxBitLen = max(BitLen, bf.BitLen);

	TBitField tmp(MaxBitLen);
	if (MemLen >= bf.MemLen) {
		for (size_t i = 0; i < bf.MemLen; i++) {
			tmp.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (size_t i = bf.MemLen; i < MemLen; i++) {
			tmp.pMem[i] = pMem[i];
		}
	}
	else{
		for (size_t i = 0; i < MemLen; i++) {
			tmp.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (size_t i = MemLen; i < bf.MemLen; i++) {
			tmp.pMem[i] = bf.pMem[i];
		}
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{

	size_t MaxBitLen = max(BitLen, bf.BitLen);

	TBitField tmp(MaxBitLen);
	if (MemLen >= bf.MemLen) {
		for (size_t i = 0; i < bf.MemLen; i++) {
			tmp.pMem[i] = pMem[i] & bf.pMem[i];
		}
		for (size_t i = bf.MemLen; i < MemLen; i++) {
			tmp.pMem[i] = 0;
		}
	}
	else {
		for (size_t i = 0; i < MemLen; i++) {
			tmp.pMem[i] = pMem[i] & bf.pMem[i];
		}
		for (size_t i = MemLen; i < bf.MemLen; i++) {
			tmp.pMem[i] = 0;
		}
	}
	return tmp;

}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);

	for (size_t i = 0; i < MemLen; i++) {
		tmp.pMem[i] = ~pMem[i];
	}

	if (BitLen > 0) {
		size_t countAllBits = MemLen * (sizeof(TELEM) * 8);
		for (size_t i = BitLen; i < countAllBits; i++) {
			tmp.pMem[MemLen - 1] &= ~tmp.GetMemMask(i);
		}
	}

	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string str; 
	istr >> str;

	for (size_t i = 0; i < bf.MemLen; i++) {
		bf.pMem[i] = 0;
	}

	size_t len = min((size_t)bf.BitLen, str.length());

	for (size_t i = 0; i < len; i++) {
		if (str[i] == '1') bf.SetBit(i);
		else if (str[i] == '0') bf.ClrBit(i);
		else break;
	}
	
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
