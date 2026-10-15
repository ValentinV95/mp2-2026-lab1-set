// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
TBitField::TBitField(int len)
{
  if (len < 0)
    throw std::invalid_argument("negative length");
  BitLen = len;
  MemLen = (len + sizeof(TELEM) * 8 - 1) / sizeof(TELEM) * 8;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / sizeof(TELEM) * 8;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return (TELEM)1 << (n % sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen) 
    throw std::out_of_range("bit index out of range");
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen)
    throw std::out_of_range("bit index out of range");
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf)
  {
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
      pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField res(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  for (int i = 0; i < MemLen; i++)
    res.pMem[i] = pMem[i];
  for (int i = 0; i < bf.MemLen; i++)
    res.pMem[i] |= bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField res(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  int minMem = MemLen < bf.MemLen ? MemLen : bf.MemLen;
  for (int i = 0; i < minMem; i++)
    res.pMem[i] = pMem[i] & bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for (int i = 0; i < BitLen; i++)
    if (!GetBit(i))
      res.SetBit(i);
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  char c;
  int i = 0;
  while (i < bf.GetLength() && (istr >> c))
  {
    if (c == '0')      bf.ClrBit(i++);
    else if (c == '1') bf.SetBit(i++);
    else break;
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.GetLength(); i++)
    ostr << bf.GetBit(i);
  return ostr;
}
