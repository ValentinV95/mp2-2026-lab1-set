#ifndef __SET_H__
#define __SET_H__

#include <iostream>
#include <stdexcept>
#include "tbitfield.h"

class TSet
{
private:
  int MaxPower;
  TBitField BitField;

  void CheckRange(int Elem) const
  {
    if (Elem < 0 || Elem >= MaxPower)
      throw std::out_of_range("Set element out of universe range");
  }

public:
  explicit TSet(int mp = 0);
  TSet(const TSet &s);
  TSet(const TBitField &bf);

  operator TBitField();

  int  GetMaxPower(void) const;
  int  IsMember(const int Elem) const;
  void InsElem(const int Elem);
  void DelElem(const int Elem);

  int       operator== (const TSet &s) const;
  int       operator!= (const TSet &s) const;
  TSet&     operator=  (const TSet &s);

  TSet operator+ (const int Elem);
  TSet operator- (const int Elem);

  TSet operator+ (const TSet &s);
  TSet operator* (const TSet &s);
  TSet operator~ (void);

  friend std::istream &operator>>(std::istream &istr, TSet &s);
  friend std::ostream &operator<<(std::ostream &ostr, const TSet &s);
};

#endif
