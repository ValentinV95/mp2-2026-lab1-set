// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{

}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
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
    if (BitField.GetBit(Elem) == 1) return 1;
    else return 0;
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
    MaxPower = s.GetMaxPower();
    BitField = s.BitField;

    return(*this);
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (MaxPower == s.MaxPower && BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(MaxPower == s.MaxPower && BitField == s.BitField);

}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet tmp(BitField | s.BitField);
    return tmp;
    
}

    TSet TSet::operator+(const int Elem) // объединение с элементом
    {
        TSet tmp(MaxPower);
        tmp.BitField = BitField;
        tmp.InsElem(Elem);
        return tmp;
    }

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(MaxPower);
    tmp.BitField = BitField;
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet tmp(BitField & s.BitField);
    return tmp;

}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(~BitField);
    return tmp;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    for (int i = 0; i < s.GetMaxPower(); ++i) {
        s.DelElem(i);
    }

    int temp;
    while (istr >> temp) {
        if (temp >= 0 && temp < s.GetMaxPower()) {
            s.InsElem(temp);
        }
    }

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (size_t i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i) == 1) ostr << i << " ";
    }
    return ostr;
}
