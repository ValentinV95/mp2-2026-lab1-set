// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len)
{
	if (len < 0)
        throw std::invalid_argument("Negative length");
  	BitLen = len;
  	MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
  	pMem = new TELEM[MemLen];

  	for (int i = 0; i < MemLen; i++)
    	pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  	BitLen = bf.BitLen;
  	MemLen = bf.MemLen;
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
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("out of range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("out of range");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw std::out_of_range("out of range");
  	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		delete[] pMem;
		BitLen = bf.BitLen;
  		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];

		for (int i = 0; i < MemLen; i++)
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
  	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = 0;
	if (BitLen > bf.BitLen)
		maxBitLen = BitLen;
	else
		maxBitLen = bf.BitLen;

	TBitField result(maxBitLen);

	for (int i = 0; i < maxBitLen; i++) {
		int b1 = 0;
		int b2 = 0;

		if (i < BitLen)
			b1 = GetBit(i);
		
		if (i < bf.BitLen)
			b2 = bf.GetBit(i);
		
		if (b1 | b2)
			result.SetBit(i);
	}
		

	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen = 0;
	if (BitLen > bf.BitLen)
		maxBitLen = BitLen;
	else
		maxBitLen = bf.BitLen;

	TBitField result(maxBitLen);

	for (int i = 0; i < maxBitLen; i++) {
		int b1 = 0;
		int b2 = 0;

		if (i < BitLen)
			b1 = GetBit(i);
		
		if (i < bf.BitLen)
			b2 = bf.GetBit(i);
		
		if (b1 & b2)
			result.SetBit(i);
	}

	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);

	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) == 0)
			result.SetBit(i);
		else
			result.ClrBit(i);
	}
	return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char b;

    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> b;
        if (b == '1')
            bf.SetBit(i);
        else if (b == '0')
            bf.ClrBit(i);
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
        ostr << bf.GetBit(i);

    return ostr;
}
