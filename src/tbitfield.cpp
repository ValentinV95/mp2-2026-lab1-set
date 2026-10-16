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
		throw std::invalid_argument("negative size");
	}
	BitLen = len;
	MemLen = (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM)*8); //добваляем битов как в TELEM но -1, для округления вверх
	pMem = new TELEM[MemLen];
	for (int i = 0;i < MemLen;i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen;i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("incorrect number");
	}
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("incorrect number");
	}
	return (static_cast<TELEM>(1) << (n % (sizeof(TELEM) * 8))); //маска в пределах байта
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("incorrect number");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("incorrect number");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("incorrect number");
	}
	return ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) {
		return *this;
	}
	else
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0;i < MemLen;i++)
			pMem[i] = bf.pMem[i];
		return *this;
	}
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	//все кроме последнего
	for (int i = 0;i < MemLen - 1;i++)
		if (pMem[i] != bf.pMem[i])
			return 0;

	//последний тк неполный может быть
	for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen;i++)
		if (GetBit(i) != bf.GetBit(i))
			return 0;

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int MinMemLen = min(MemLen, bf.MemLen);
	int MaxBitLen = max(BitLen, bf.BitLen);
	TBitField result_field(MaxBitLen);

	for (int i = 0;i < MinMemLen;i++)
		result_field.pMem[i] = pMem[i] | bf.pMem[i];

	if (MemLen > bf.MemLen) {
		for (int i = MinMemLen;i < MemLen;i++)
			result_field.pMem[i] = pMem[i];
	}
	else if(bf.MemLen > MemLen) {
		for (int i = MinMemLen;i < bf.MemLen;i++)
			result_field.pMem[i] = bf.pMem[i];
	}
	return result_field;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int MinMemLen = min(MemLen, bf.MemLen);
	int MaxBitLen = max(BitLen, bf.BitLen);
	TBitField result_field(MaxBitLen);

	for (int i = 0;i < MinMemLen;i++)
		result_field.pMem[i] = pMem[i] & bf.pMem[i];

	return result_field;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result_field(BitLen);
	for (int i = 0;i < MemLen; i++)
		result_field.pMem[i] = ~pMem[i];

	unsigned int lastElemBits = BitLen % (sizeof(TELEM) * 8);
	if (lastElemBits != 0){
		TELEM mask = (static_cast<TELEM>(1) << lastElemBits)-1;
		result_field.pMem[MemLen - 1] &= mask;
	}

	return result_field;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char digit;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> digit;
		if (digit == '0')
			bf.ClrBit(i);
		else if (digit == '1')
			bf.SetBit(i);
		else
			break;
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen;i++)
		ostr << bf.GetBit(i);

	return ostr;
}
