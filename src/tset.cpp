// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) { }

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
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
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s)
        return *this;

    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower)
        return 0;
    if (BitField != s.BitField)
        return 0;
    return 1;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int mp = (MaxPower >= s.MaxPower) ? MaxPower : s.MaxPower;
    TSet Res(mp);
    Res.BitField = BitField | s.BitField;
    return Res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet Res(*this);
    Res.InsElem(Elem);
    return Res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet Res(*this);
    Res.DelElem(Elem);
    return Res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

//{1, 2, 3} or 1, 2, 3 or 1 2 3
istream &operator>>(istream &istr, TSet &s) // ввод
{
    char symbol;

    while (istr >> symbol) 
    {
        if ((symbol == '{') || (symbol == '}') || (symbol == ','))
            continue;
        else 
        {
            istr.putback(symbol);
            int i;
            istr >> i;
            s.InsElem(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{";

    int max_el = -1;
    for (int j = s.GetMaxPower() - 1; j >= 0; j--)
    {
        if (s.IsMember(j)) 
        {
            max_el = j;
            break;
        }
    }

    if (max_el == -1) {
        ostr << "}";
        return ostr;
    }

    for (int i = 0; i < max_el; i++)
    {
        if (s.IsMember(i))
        {
            ostr << i << ", ";
        }
    }
    ostr << max_el << "}";

    return ostr;
}
