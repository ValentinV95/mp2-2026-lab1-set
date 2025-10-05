// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) {} //создаем битовое поле и запоминаем мощность множества

// конструктор копирования
TSet::TSet(const TSet& otherset) : BitField(otherset.BitField), MaxPower(otherset.MaxPower) {}
//копируем мощность множества и вызываем конструктор копирования

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()) {}
//мощность множества - длина битового поля, копируем битовое поле

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
    if ((MaxPower <= Elem) || (Elem < 0)) { throw std::out_of_range("The index is wrong"); }

    return BitField.GetBit(Elem); //вернёт 1, если есть в множестве, иначе - 0
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((MaxPower <= Elem) || (Elem < 0)) { throw std::out_of_range("The index is wrong"); }

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((MaxPower <= Elem) || (Elem < 0)){ throw std::out_of_range("The index is wrong"); }

    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) 
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField; //оператор = у TBitField
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) { return false; } //если мощности разные, то множества не равны

    if (BitField == s.BitField) { return true; }//оператор == из TBitField
    else { return false; } //биты различаются в остальных случаях
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int NewPower;
    if (MaxPower > s.MaxPower) { NewPower = MaxPower; }//выбираем большую мощность из двух
    else { NewPower = s.MaxPower; }
    TSet result(NewPower);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet result(*this);
    result.InsElem(Elem); //добавляем элемент
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);
    result.DelElem(Elem); //удаляем элемент
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int NewPower;
    if (MaxPower > s.MaxPower) { NewPower = MaxPower; } //выбираем множество с большей мощностью
    else { NewPower = s.MaxPower; }
    TSet result(NewPower);
    result.BitField = BitField & s.BitField; //возвращаем битовое поле максимальной длины 
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
