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
	if (Elem < 0 || Elem > MaxPower) throw std::out_of_range("Invalid index");
	else return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	if (Elem < 0 || Elem > MaxPower) throw std::out_of_range("Invalid index");
	else return BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	if (Elem < 0 || Elem > MaxPower) throw std::out_of_range("Invalid index");
	else return BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
	if (this != &s) {
		MaxPower = s.MaxPower;
		BitField = s.BitField;
	}
	return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
	if (MaxPower == s.MaxPower) {
		if (BitField == s.BitField) return 1;
		else return 0;
	}
	else return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
	if (MaxPower == s.MaxPower) {
		if (BitField == s.BitField) return 0;
		else return 1;
	}
	else return 1;
}

TSet TSet::operator+(const TSet& s) // объединение
{
	int newPower = max(MaxPower, s.MaxPower);
	TSet newSet(newPower);
	newSet.BitField = BitField | s.BitField;
	return newSet;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	int newPower = 0;
	if (Elem > MaxPower) newPower = Elem + 1;
	else newPower = MaxPower;

	TSet newField(newPower);
	newField.BitField = BitField;
	newField.InsElem(Elem);
	return newField;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	TSet newSet(*this);
	newSet.DelElem(Elem);
	return newSet;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
	int newPower = max(MaxPower, s.MaxPower);
	TSet newSet(newPower);
	newSet.BitField = BitField & s.BitField;
	return newSet;
}

TSet TSet::operator~(void) // дополнение
{
	TSet newSet(MaxPower);
	newSet.BitField = ~BitField;
	return newSet;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
	int elem;
	char ch;
	for (int i = 0; i < s.MaxPower; i++) s.DelElem(i);
	while (istr >> elem)
	{
		if (elem >= 0 && elem < s.MaxPower) s.InsElem(elem);
		else throw std::invalid_argument("Invalid element");

		ch = istr.peek();
		if (ch == '\n') break;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
	ostr << "{";
	for (int i = 0; i < s.MaxPower; i++)
	{
		if (s.IsMember(i)) {
			if (i != s.MaxPower - 1) ostr << i << ", ";
			else ostr << i;
		}
	}
	ostr << "}";
	return ostr;
}