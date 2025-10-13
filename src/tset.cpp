// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(0)
{
    MaxPower = mp;
    BitField = TBitField(mp);
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(0)
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(0)
{
    MaxPower = bf.GetLength();
    BitField = bf;

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
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField==s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet t(BitField | s.BitField);
    return t;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet t(Elem);
    if (Elem < MaxPower) {
        t=*this;
        t.InsElem(Elem);
    }
    else {
        t.InsElem(Elem);
        t = t + (*this);
    }
    return t;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet t(*this);
    t.DelElem(Elem);
    return t;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet t(*this);
    t.BitField = BitField & s.BitField;
    return t;
}

TSet TSet::operator~(void) // дополнение
{
    TSet t(*this);
    t.BitField = ~BitField;
    return t;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int a;
    while (istr >> a)s.InsElem(a);
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i))ostr << i;
    }
    return ostr;
}
