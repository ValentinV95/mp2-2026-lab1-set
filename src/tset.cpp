// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
	TSet::MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
	TSet::MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    TSet::MaxPower = bf.GetLength();
}

TSet::operator TBitField() 
{
	TBitField bf(TSet::BitField);
    return bf;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return TSet::MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= TSet::MaxPower)
        return 0;
	if (TSet::BitField.GetBit(Elem))
		return 1;
    return 0;
    //return FAKE_INT;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= TSet::MaxPower)
        throw "Error: out of range pMem";
    TSet::BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= TSet::MaxPower)
        throw "Error: out of range pMem";
    TSet::BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
	TSet::MaxPower = s.MaxPower;
	TSet::BitField = s.BitField;
	return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
	if (TSet::MaxPower != s.MaxPower)
		return 0;
	if (TSet::BitField == s.BitField)
		return 1;
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this==s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
	TSet res(BitField | s.BitField);
	return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	TSet res(*this);
	res.InsElem(Elem);   // InsElem сам бросит исключение, если Elem вне диапазона
	return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	/*if (Elem < 0 || Elem >= TSet::MaxPower)
		throw "Error: out of range pMem";*/
	if (!TSet::IsMember(Elem))
		return *this;
	TSet res(*this);
	res.DelElem(Elem);   // DelElem сам бросит исключение, если Elem вне диапазона
	return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
	TSet res(BitField & s.BitField);
	return res;
}

TSet TSet::operator~(void) // дополнение
{
	return TSet(~TSet::BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
	istr >>s.MaxPower >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    return ostr;
}
