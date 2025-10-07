#include "tset.h"

using std::ostream;
using std::istream;

TSet::TSet(int mp)
  : MaxPower(mp), BitField(mp)
{
  if (mp < 0) throw std::invalid_argument("Set max power must be non-negative");
}

TSet::TSet(const TSet &s)
  : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField &bf)
  : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
  return BitField;
}

int TSet::GetMaxPower(void) const
{
  return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
  CheckRange(Elem);
  return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
  CheckRange(Elem);
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
  CheckRange(Elem);
  BitField.ClrBit(Elem);
}

int TSet::operator==(const TSet &s) const
{
  return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const
{
  return !(*this == s);
}

TSet& TSet::operator=(const TSet &s)
{
  if (this == &s) return *this;
  MaxPower = s.MaxPower;
  BitField = s.BitField;
  return *this;
}

TSet TSet::operator+(const int Elem)
{
  TSet res(*this);
  res.InsElem(Elem);
  return res;
}

TSet TSet::operator-(const int Elem)
{
  TSet res(*this);
  res.DelElem(Elem);
  return res;
}

TSet TSet::operator+(const TSet &s)
{
  TBitField a = this->BitField;
  TBitField b = s.BitField;
  TSet res( (a.GetLength() > b.GetLength()) ? a.GetLength() : b.GetLength() );
  res.BitField = a | b;
  return res;
}

TSet TSet::operator*(const TSet &s)
{
  TBitField a = this->BitField;
  TBitField b = s.BitField;
  TSet res( (a.GetLength() > b.GetLength()) ? a.GetLength() : b.GetLength() );
  res.BitField = a & b;
  return res;
}

TSet TSet::operator~(void)
{
  TSet res(*this);
  res.BitField = ~res.BitField;
  return res;
}

istream &operator>>(istream &istr, TSet &s)
{
  for (int i = 0; i < s.GetMaxPower(); ++i) s.BitField.ClrBit(i);

  char ch;
  while (istr >> ch) { if (!std::isspace(static_cast<unsigned char>(ch))) break; }
  if (!istr) return istr;
  if (ch != '{') { istr.setstate(std::ios::failbit); return istr; }

  int x;
  while (istr >> x) {
    if (0 <= x && x < s.MaxPower) s.BitField.SetBit(x);
    char c = 0;
    if (!(istr >> c)) break;
    if (c == '}') break;
    if (c != ' ') istr.unget();
  }
  return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s)
{
  ostr << "{";
  bool first = true;
  for (int i = 0; i < s.MaxPower; ++i) {
    if (s.BitField.GetBit(i)) {
      if (!first) ostr << " ";
      first = false;
      ostr << i;
    }
  }
  ostr << "}";
  return ostr;
}
