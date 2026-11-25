// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string> 
#include <stdexcept> 

TBitField::TBitField(int len)
{
  if (len < 0) throw std::invalid_argument("length cannot be less zero");
  BitLen = len;
  MemLen = (len + (int)sizeof(TELEM)*8 - 1)/(int)(sizeof(TELEM)*8);
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[bf.MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
  pMem = 0;
  BitLen = 0;
  MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
  return n / (int)(sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
  return (TELEM)1 << (n % (int)(sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) && 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf) return *this;
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[bf.MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
  return *this; 
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) return 0;
  for (int i = 0; i < MemLen; i++) if (pMem[i] != bf.pMem[i]) return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) return 1;
  for (int i = 0; i < MemLen; i++) if (pMem[i] != bf.pMem[i]) return 1;
  return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{

  int resLen = 0;
  if (BitLen > bf.BitLen) resLen = BitLen;
  else resLen = bf.BitLen;
  TBitField res(resLen);

  int minMemLen = 0;
  if (MemLen < bf.MemLen) minMemLen = MemLen;
  else minMemLen = bf.MemLen;
  for (int i = 0; i < minMemLen; i++) res.pMem[i] = pMem[i] | bf.pMem[i];

  if (MemLen > bf.MemLen) for (int i = minMemLen; i < res.MemLen; i++) res.pMem[i] = pMem[i];
  else for (int i = minMemLen; i < res.MemLen; i++) res.pMem[i] = bf.pMem[i];

  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int resLen = 0;
  if (BitLen > bf.BitLen) resLen = BitLen;
  else resLen = bf.BitLen;
  TBitField res(resLen);

  int minMemLen = 0;
  if (MemLen < bf.MemLen) minMemLen = MemLen;
  else minMemLen = bf.MemLen;
  for (int i = 0; i < minMemLen; i++) res.pMem[i] = pMem[i] & bf.pMem[i];

  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen; i++) res.pMem[i] = ~pMem[i];
  int unusedBits = BitLen % (int)(sizeof(TELEM)*8);
  if (unusedBits != 0) 
      res.pMem[res.MemLen - 1] &= ((TELEM)1 << unusedBits) - 1;
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string s;
  istr >> s;
  for (int i = 0; i < bf.BitLen; i++) bf.ClrBit(i);
  int j = 0;
  for ( int i = 0; i < bf.BitLen && j < s.size(); i++) {
    if (s[j] == '0') {bf.ClrBit(j); j++;}
    else if (s[j] == '1') {bf.SetBit(j); j++;}
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++) ostr << bf.GetBit(i);
  return ostr;
}
