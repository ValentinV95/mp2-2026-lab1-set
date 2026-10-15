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
	if (len <= 0)
	{
		throw invalid_argument("Error: Negative number");
	}
	
	BitLen = len;
	MemLen = (BitLen + (8 * sizeof(TELEM) - 1)) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) 
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8 *sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1u << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) 
	{
		throw out_of_range("Error: Index out of range");
	}

	int NInd = GetMemIndex(n);
	TELEM NMask = GetMemMask(n);

	pMem[NInd] |= NMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
	{
		throw out_of_range("Error: Index out of range");
	}

	int NInd = GetMemIndex(n);
	TELEM NMask = GetMemMask(n);

	pMem[NInd] &= ~(NMask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
	{
		throw out_of_range("Error: Index out of range");
	}

	int NInd = GetMemIndex(n);
	TELEM NMask = GetMemMask(n);

	TELEM check_value = pMem[NInd] & NMask;
	
	return (check_value == 0) ? 0: 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) 
		return *this;

	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	delete[] pMem;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) 
	{
		pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	
	for (int i = 0; i < MemLen; i++) 
	{
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
	int MaxBitLen = (BitLen >= bf.BitLen) ? BitLen : bf.BitLen;

	TBitField Res(MaxBitLen);

	for (int i = 0; i < Res.MemLen; i++) 
	{
		if (i >= MemLen)
			Res.pMem[i] = bf.pMem[i];
		else if (i >= bf.MemLen)
			Res.pMem[i] = pMem[i];
		else
			Res.pMem[i] = pMem[i] | bf.pMem[i];
	}

	return Res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int MaxBitLen = (BitLen >= bf.BitLen) ? BitLen : bf.BitLen;

	TBitField Res(MaxBitLen);

	for (int i = 0; i < Res.MemLen; i++)
	{
		if ((i < MemLen) && (i < bf.MemLen))
			Res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	
	return Res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField Res(BitLen);

	for (int i = 0; i < MemLen - 1; i++)
		Res.pMem[i] = ~pMem[i];

	if (BitLen % (8 * sizeof(TELEM)) == 0)
		Res.pMem[MemLen - 1] = ~pMem[MemLen - 1];

	else 
	{
		Res.pMem[MemLen - 1] = ~pMem[MemLen - 1];
		TELEM res_mask = 0;

		unsigned int significants_bits = BitLen - (MemLen - 1) * 8 * sizeof(TELEM);
		for (int i = 0; i < significants_bits; i++)
		{
			res_mask |= GetMemMask(i);
		}

		Res.pMem[MemLen - 1] &= res_mask;
	}

	return Res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	
	for (int i = bf.BitLen - 1; i >= 0; i--)
	{
		char symbol;
		istr >> symbol;

		if (symbol == '1')
			bf.SetBit(i);
		else if (symbol == '0')
			bf.ClrBit(i);
		else 
		{
			throw invalid_argument("Error: this is not BitField");
		}
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
