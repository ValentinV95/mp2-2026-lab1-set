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
  if (len < 0) {
    throw std::invalid_argument("Отрицательная длина");
  }
  BitLen = len;
  MemLen = len % 32 == 0 ? len/32 : (len/32)+1;
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; i++) { pMem[i] = 0; } 
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
  }
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1u << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Неправильный индекс");
  }
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Неправильный индекс");
  }
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Неправильный индекс");
  }
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf) {
    return *this;
  }
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;

  pMem = new TELEM[bf.MemLen];

  for (int i = 0; i < bf.MemLen; i++){
    pMem[i] = bf.pMem[i];
  }

  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) { return 0; }
  for (int i = 0; i < bf.MemLen; i++) {
    if (pMem[i] != bf.pMem[i]) { return 0; }
  }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int maxl = BitLen > bf.BitLen ? BitLen : bf.BitLen;

  TBitField res(maxl);
  int minpmem = MemLen > bf.MemLen ? bf.MemLen : MemLen;
  for (int i = 0; i < minpmem; i++) {
    res.pMem[i] = pMem[i] | bf.pMem[i];
  }
  if (MemLen > bf.MemLen) {
    for (int i = minpmem; i < MemLen; i++) { res.pMem[i] = pMem[i]; }
  }
  else if (bf.MemLen > MemLen) {
    for (int i = minpmem; i < bf.MemLen; i++) { res.pMem[i] = bf.pMem[i]; }
  }
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
  TBitField res(maxLen);

  int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
  
  for (int i = 0; i < minMemLen; i++) {
    res.pMem[i] = bf.pMem[i] & pMem[i];
  }

  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField ob_pMem(BitLen);
  for (int i = 0; i < MemLen; i++) {
    ob_pMem.pMem[i] = ~pMem[i];
  }

  if (BitLen % 32 != 0) { ob_pMem.pMem[MemLen-1] &= ((1u << BitLen % 32) - 1); }

  return ob_pMem;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  for (int i = 0; i < bf.BitLen; i++) {
    char bit;
    istr >> bit;
    if (!istr) {
      break;
    }
    
    if (bit == '1') {
      bf.SetBit(i);
    }
    else if (bit == '0') {
      bf.ClrBit(i);
    }
  }

  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.GetLength(); i++) {
    ostr << bf.GetBit(i);
  }

  return ostr;
}