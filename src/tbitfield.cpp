// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "invalid length";
	BitLen = len;
	MemLen = len / (sizeof(TELEM) * 8);
	if (len != MemLen * (sizeof(TELEM) * 8)) { MemLen++; }
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / (sizeof(TELEM) * 8));
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
	if (n < 0 || n >= BitLen) throw "invalid index";
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) throw "invalid index";
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) throw "invalid index";
	return (pMem[GetMemIndex(n)] & GetMemMask(n))!=0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) { return *this; }
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (bf.BitLen != BitLen) { return false; }
	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return false;
		}
	}
	return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField temp(max(bf.BitLen, BitLen));
	for (int i = 0; i < BitLen; i++)
	{
		if ((*this).GetBit(i))
		{
			temp.SetBit(i);
		}
	}
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i))
		{
			temp.SetBit(i);
		}
	}
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField temp(max(bf.BitLen, BitLen));
	int minlen = min(bf.BitLen, BitLen);
	for (int i = 0; i < minlen; i++)
	{
		if ((*this).GetBit(i) && bf.GetBit(i))
		{
			temp.SetBit(i);
		}
	}
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	for (int i = 0; i < BitLen; i++)
	{
		if (temp.GetBit(i)) { temp.ClrBit(i); }
		else { temp.SetBit(i); }
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string s;
	istr >> s;
	for (int i = 0; i < s.size() && i < bf.BitLen; i++)
	{
		if (s[i] == '0') { bf.ClrBit(i); }
		else { bf.SetBit(i); }
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << (bf.GetBit(i) ? '1' : '0');
	}
	return ostr;
}
