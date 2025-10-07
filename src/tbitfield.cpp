#include "tbitfield.h"

using std::ostream;
using std::istream;

int TBitField::BitsPerElem() { return static_cast<int>(sizeof(TELEM) * 8); }

TBitField::TBitField(int len)
  : BitLen(len), pMem(nullptr), MemLen(0)
{
  if (len < 0)
    throw std::invalid_argument("Bit field length must be non-negative");
  MemLen = (BitLen + BitsPerElem() - 1) / BitsPerElem();
  if (MemLen > 0) {
    pMem = new TELEM[MemLen];
    std::memset(pMem, 0, sizeof(TELEM) * MemLen);
  }
}

TBitField::TBitField(const TBitField &bf)
  : BitLen(bf.BitLen), pMem(nullptr), MemLen(bf.MemLen)
{
  if (MemLen > 0) {
    pMem = new TELEM[MemLen];
    std::memcpy(pMem, bf.pMem, sizeof(TELEM) * MemLen);
  }
}

TBitField::~TBitField()
{
  delete [] pMem;
  pMem = nullptr;
  MemLen = 0;
  BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const
{
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("Bit index out of range");
  return n / BitsPerElem();
}

TELEM TBitField::GetMemMask(const int n) const
{
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("Bit index out of range");
  int offset = n % BitsPerElem();
  return static_cast<TELEM>(1u) << offset;
}

int TBitField::GetLength(void) const
{
  return BitLen;
}

void TBitField::SetBit(const int n)
{
  int idx = GetMemIndex(n);
  pMem[idx] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
  int idx = GetMemIndex(n);
  pMem[idx] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
  int idx = GetMemIndex(n);
  return (pMem[idx] & GetMemMask(n)) ? 1 : 0;
}

int TBitField::operator==(const TBitField &bf) const
{
  if (BitLen != bf.BitLen) return 0;
  for (int i = 0; i < MemLen; ++i)
    if (pMem[i] != bf.pMem[i]) return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const
{
  return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField &bf)
{
  if (this == &bf) return *this;
  if (MemLen != bf.MemLen) {
    delete [] pMem;
    pMem = nullptr;
    MemLen = bf.MemLen;
    if (MemLen > 0)
      pMem = new TELEM[MemLen];
  }
  BitLen = bf.BitLen;
  if (MemLen > 0)
    std::memcpy(pMem, bf.pMem, sizeof(TELEM) * MemLen);
  return *this;
}

TBitField TBitField::operator|(const TBitField &bf) const
{
  int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
  TBitField res(newLen);
  int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
  for (int i = 0; i < minMem; ++i)
    res.pMem[i] = pMem[i] | bf.pMem[i];
  if (MemLen > bf.MemLen)
    for (int i = minMem; i < MemLen; ++i) res.pMem[i] = pMem[i];
  else
    for (int i = minMem; i < bf.MemLen; ++i) res.pMem[i] = bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) const
{
  int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
  TBitField res(newLen);
  int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
  for (int i = 0; i < minMem; ++i)
    res.pMem[i] = pMem[i] & bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) const
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen; ++i)
    res.pMem[i] = ~pMem[i];

  int extra = MemLen * BitsPerElem() - BitLen;
  if (extra > 0) {
    TELEM mask = ~static_cast<TELEM>(0);
    mask >>= extra;
    res.pMem[MemLen - 1] &= mask;
  }
  return res;
}

istream &operator>>(istream &istr, TBitField &bf)
{
  for (int i = 0; i < bf.MemLen; ++i) bf.pMem[i] = 0;

  char ch;
  while (istr >> ch) { if (!std::isspace(static_cast<unsigned char>(ch))) break; }
  if (!istr) return istr;

  if (ch != '{') {
    istr.setstate(std::ios::failbit);
    return istr;
  }

  int pos;
  while (istr >> pos) {
    if (0 <= pos && pos < bf.BitLen) bf.SetBit(pos);
    char c = 0;
    if (!(istr >> c)) break;
    if (c == '}') break;
    if (c != ' ') istr.unget();
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf)
{
  ostr << "{";
  bool first = true;
  for (int i = 0; i < bf.BitLen; ++i) {
    if (bf.GetBit(i)) {
      if (!first) ostr << " ";
      first = false;
      ostr << i;
    }
  }
  ostr << "}";
  return ostr;
}
