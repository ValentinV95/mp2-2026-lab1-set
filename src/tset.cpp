// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower)
        return 0;

    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";

    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s)
        return *this;

    MaxPower = s.MaxPower;
    BitField = s.BitField;

    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int maxPower = MaxPower;
    if (s.MaxPower > maxPower)
        maxPower = s.MaxPower;

    TSet result(maxPower);
    result.BitField = BitField | s.BitField;

    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0)
        throw "Negative element";

    int newMaxPower = MaxPower;
    if (Elem >= MaxPower)
        newMaxPower = Elem + 1;

    TSet result(newMaxPower);
    result.BitField = BitField;
    result.InsElem(Elem);

    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);

    if (Elem >= 0 && Elem < MaxPower)
        result.DelElem(Elem);

    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int maxPower = MaxPower;
    if (s.MaxPower > maxPower)
        maxPower = s.MaxPower;

    TSet result(maxPower);
    result.BitField = BitField & s.BitField;

    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;

    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int elem;
    char ch;

    do
    {
        istr >> elem;
        if (elem >= 0 && elem < s.MaxPower)
            s.InsElem(elem);
        istr >> ch;
    } while (ch == ',');

    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << "{";
    int first = 1;

    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.IsMember(i))
        {
            if (!first)
                ostr << ", ";
            ostr << i;
            first = 0;
        }
    }

    ostr << "}";

    return ostr;
}