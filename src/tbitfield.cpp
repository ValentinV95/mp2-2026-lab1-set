// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests


//Is done


static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len < 0)
		throw "Error: negative length of bitfield";
	TBitField::BitLen = len;
	TBitField::MemLen = (len - 1) / (sizeof(TELEM) * 8) + 1;
	TBitField::pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)  //обнуление 
	{
		TBitField::pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	TBitField::BitLen = bf.BitLen;
	TBitField::MemLen = bf.MemLen;
	TBitField::pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		TBitField::pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] TBitField::pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита 
{
	if (n >= BitLen)
		throw "Error: out of range pMem";

	return static_cast<int>(n / (sizeof(TELEM) * 8));  //sizeof(telem) = 4 b

	//return FAKE_INT;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= BitLen)
		throw "Error: out of range pMem";

	return static_cast<TELEM>(1 << (n % (sizeof(TELEM) * 8)));  //one TELEM not a full pMem
	//return FAKE_INT;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
	//return FAKE_INT;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
		throw "Error: out of range pMem";
	this->pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
		throw "Error: out of range pMem";
	this->pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw "Error: out of range pMem";
	if (this->pMem[GetMemIndex(n)] & GetMemMask(n))
		return 1;
	else
		return 0;
	//return FAKE_INT;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf)
{
	if (this == &bf)
		return *this;
	if (MemLen != bf.MemLen)
	{
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}
int TBitField::operator==(const TBitField &bf) const // сравнение // чекнуть ответ от полины
{
	bool flag = true;
	if (BitLen != bf.BitLen || MemLen != bf.MemLen)
		flag = false;
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i])
			flag = false;
	}
	if (flag)
		return 1;
	return 0;
	//return FAKE_INT;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf); 
    //return FAKE_INT;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"  
{

	int minsize = (TBitField::MemLen < bf.MemLen) ? TBitField::MemLen : bf.MemLen;
	int maxsize = (TBitField::MemLen > bf.MemLen) ? TBitField::MemLen : bf.MemLen;
	TBitField res(maxsize * sizeof(TELEM) * 8);
	if (TBitField::MemLen > bf.MemLen) {
		minsize = bf.MemLen;
		res = TBitField(*this);
	}
	else {
		minsize = TBitField::MemLen;
		res = TBitField(bf);
	}
	
	
	for (int i = 0; i < minsize; i++) {
		res.pMem[i] = TBitField::pMem[i] | bf.pMem[i];
	}
	return res;
    //return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField res(len);                      
	int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
	for (int i = 0; i < minMem; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			res.SetBit(i);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод    
{
	for (int i = 0; i < bf.MemLen; i++) {
		istr >> bf.pMem[i];
	}
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.MemLen; i++)
	{
		ostr << bf.pMem[i] << " ";
	}
    return ostr;
}
