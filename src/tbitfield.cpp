// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <stdexcept>
#include "tbitfield.h"

TBitField::TBitField(int len) {
  if (len < 0)
    throw std::exception("Negative size");
  else
    BitLen = len;

  MemLen = (BitLen + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);

  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) {
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() {
  delete[] pMem;
  pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const {
  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const {
  return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

int TBitField::GetLength(void) const { return BitLen; }

void TBitField::SetBit(const int n) {
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("Index out of range");

  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) {
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("Index out of range");

  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const {
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("Index out of range");

  return pMem[GetMemIndex(n)] & GetMemMask(n);
}

TBitField &TBitField::operator=(const TBitField &bf) {
  if (this == &bf) return *this;

  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const {
  if (BitLen != bf.BitLen) return 0;

  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i]) return 0;

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const { return !(*this == bf); }

TBitField TBitField::operator|(const TBitField &bf) {
  int maxBitLen = std::max(BitLen, bf.BitLen);
  TBitField result(maxBitLen);
  const TBitField *longer;
  const TBitField *shorter;

  if (MemLen > bf.MemLen) {
    longer = this;
    shorter = &bf;
  } else {
    longer = &bf;
    shorter = this;
  }

  for (int i = 0; i < shorter->MemLen; i++)
    result.pMem[i] = shorter->pMem[i] | longer->pMem[i];

  for (int i = shorter->MemLen; i < longer->MemLen; i++)
    result.pMem[i] = longer->pMem[i];

  return result;
}

TBitField TBitField::operator&(const TBitField &bf) {
  int maxBitLen = std::max(BitLen, bf.BitLen);
  int minMemLen = std::min(MemLen, bf.MemLen);

  TBitField result(maxBitLen);

  for (int i = 0; i < minMemLen; i++) result.pMem[i] = pMem[i] & bf.pMem[i];

  return result;
}

TBitField TBitField::operator~(void) {
  TBitField result(BitLen);

  for (int i = 0; i < MemLen; i++) result.pMem[i] = ~pMem[i];

  int tailBits = BitLen % (sizeof(TELEM) * 8);

  if (tailBits != 0) {
    TELEM clearMask = ((TELEM)1 << tailBits) - 1;
    result.pMem[MemLen - 1] &= clearMask;
  }

  return result;
}

istream &operator>>(istream &istr, TBitField &bf) {
  std::string s;
  if (!(istr >> s)) return istr;

  if (bf.pMem != nullptr) {
    delete[] bf.pMem;
    bf.pMem = nullptr;
  }

  bf.BitLen = s.length();
  int bitsPerElem = sizeof(TELEM) * 8;
  bf.MemLen = (bf.BitLen + bitsPerElem - 1) / bitsPerElem;

  bf.pMem = new TELEM[bf.MemLen];
  for (int i = 0; i < bf.MemLen; i++) bf.pMem[i] = 0;

  for (int i = 0; i < bf.BitLen; i++)
    if (s[i] == '1') bf.SetBit(i);

  return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) {
  for (int i = 0; i < bf.BitLen; i++)
    ostr << (bf.GetBit(i) ? '1' : '0');

  return ostr;
}
