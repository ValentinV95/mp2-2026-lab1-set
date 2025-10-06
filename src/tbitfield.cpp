// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include "tbitfield.h"
#define Size_TELEM (8 * sizeof(TELEM)) 


TBitField::TBitField(int len)
{
	if (len < 0)
	{
		throw out_of_range(" len must be >= 0"); // ПРОВЕРКА 1
	}
	BitLen = len;
	MemLen = (BitLen + Size_TELEM - 1) / Size_TELEM;
	pMem = (new TELEM[MemLen]);
	for (int n = 0; n < MemLen; n++) {
		pMem[n] = 0;
	}
}


TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = (new TELEM[MemLen]);
	for (int n = 0; n < MemLen; n++) {
		pMem[n] = bf.pMem[n];
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] (pMem);
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / Size_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % Size_TELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{	
  return BitLen;
}


void TBitField::SetBit(const int n) // установить бит
{
	if (n<0 || n>BitLen)
	{
		throw out_of_range("out of range");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>BitLen)
	{
		throw out_of_range("out of range");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>BitLen)
	{
		throw out_of_range("out of range");
	}
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) {
		return *this;
	}

	free(pMem);
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = (new TELEM[MemLen]);
	for (int n = 0; n < MemLen; n++) {
		pMem[n] = bf.pMem[n];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (bf.GetLength() != BitLen) {
		return false;
	}
	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return (1+operator==(bf))%2;
}

TBitField TBitField::operator|(const TBitField &bf)  // операция "или"
{
	int len = max(bf.GetLength(), GetLength());
	TBitField temp(len);	
	for (int i = 0; i < GetLength(); i++) {
		if ((*this).GetBit(i)) {
			temp.SetBit(i);
		}
	}

	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i)) {
			temp.SetBit(i);
		}
	}

	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = max(bf.GetLength(), BitLen);
	int minlen = min(bf.GetLength(), BitLen);
	TBitField temp(len);
	for (int i = 0; i < minlen; i++) {
		if ((*this).GetBit(i) && bf.GetBit(i)) {
			temp.SetBit(i);
		}
	}

	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	for (int i = 0; i < temp.BitLen; i++)
	{
		if (temp.GetBit(i)) {
			temp.ClrBit(i);
		}
		else
		{
			temp.SetBit(i);
		}
	}
	return temp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	string input;
	istr >> input;

	for (int i = 0; i < input.size() && i < bf.GetLength(); i++) {
		if (input[i] == '1') {
			bf.SetBit(i);
		}
		else if (input[i] == '0') {
			bf.ClrBit(i);
		}
	}

	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << (bf.GetBit(i) ? '1' : '0');
	}
	ostr << " ";
	ostr << bf.MemLen;
	return ostr;
}
