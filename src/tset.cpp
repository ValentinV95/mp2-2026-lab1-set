// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include "tbitfield.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}

TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() { return BitField; }

int TSet::GetMaxPower(void) const { return MaxPower; }

int TSet::IsMember(const int Elem) const { return BitField.GetBit(Elem); }

void TSet::InsElem(const int Elem) { BitField.SetBit(Elem); }

void TSet::DelElem(const int Elem) { BitField.ClrBit(Elem); }

TSet &TSet::operator=(const TSet &s) {
  if (this == &s)
    return *this;

  MaxPower = s.MaxPower;
  BitField = s.BitField;
  return *this;
}

int TSet::operator==(const TSet &s) const {
  return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const { return !(*this == s); }

TSet TSet::operator+(const TSet &s) {
  int newMaxPower = std::max(MaxPower, s.MaxPower);
  TBitField resultBitField = BitField | s.BitField;

  return TSet(resultBitField);
}

TSet TSet::operator+(const int Elem) {
  TSet Res = *this;
  Res.InsElem(Elem);
  return Res;
}

TSet TSet::operator-(const int Elem) {
  TSet Res = *this;
  Res.DelElem(Elem);
  return Res;
}

TSet TSet::operator*(const TSet &s) {
  TBitField resultBitField = BitField & s.BitField;

  return TSet(resultBitField);
}

TSet TSet::operator~(void) { return TSet(~BitField); }

istream &operator>>(istream &istr, TSet &s) {
  istr >> s.BitField;
  return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s) {
  ostr << s.BitField;
  return ostr;
}
