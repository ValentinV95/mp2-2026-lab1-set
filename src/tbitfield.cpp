// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <algorithm>

TBitField::TBitField(int len)
{
  if (len < 0){
    throw std::exception("Length is wrong");
  }
  BitLen = len;
  MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++){
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen;i++){
    pMem[i] = bf.pMem[i];
  }
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return TELEM(1) << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Invalid index!");
  }
  pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Invalid index!");
  }
  TELEM not_n = ~GetMemMask(n);
  pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & not_n;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Invalid index!");
  }
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField &TBitField::operator=(const TBitField &bf) // присваивание
{
  if(this==&bf){
    return *this;
  }
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen;i++){
    pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if(BitLen!=bf.BitLen){
    return 0;
  }else{
    for (int i = 0; i < BitLen;i++){
      if(GetBit(i) != bf.GetBit(i)){
        return 0;
      }
    }
  }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf); 
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int max_len = max(GetLength(), bf.GetLength());
  int min_mem = min(MemLen, bf.MemLen);

  TBitField res(max_len);
  for (int i = 0; i < min_mem;i++){
    res.pMem[i] = pMem[i] | bf.pMem[i];
  }
  for (int j = min_mem; j < res.MemLen; j++) {
    if (j < MemLen) {
        res.pMem[j] = pMem[j];
    }
    else{
        res.pMem[j] = bf.pMem[j];
    }
  }
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int max_len = max(GetLength(), bf.GetLength());
  int min_mem = min(MemLen, bf.MemLen);

  TBitField res(max_len);
  for (int i = 0; i < min_mem; i++) {
      res.pMem[i] = pMem[i] & bf.pMem[i];
  }
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen;i++){
    res.pMem[i] = ~pMem[i];
  }
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  int n, k;
  istr >> n;
  bf = TBitField(n);
 
  for (int i = 0; i < bf.BitLen; i++){
    istr >> k;
    if (k == 1){
      bf.SetBit(i);
    }else{
        bf.ClrBit(i);
    }
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++){
    ostr << bf.GetBit(i);
  }
  return ostr;
}
