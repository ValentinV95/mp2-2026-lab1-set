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
        throw std::out_of_range("error");
    BitLen = len;
    int bits = sizeof(TELEM) * 8;
    MemLen = (BitLen + (bits - 1)) / bits;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
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
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("error");
    int bits = sizeof(TELEM) * 8;
    return n / bits;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("error");
    int bits = sizeof(TELEM) * 8;
    return 1 << (n % bits);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("error");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("error");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("error");
    return (pMem[GetMemIndex(n)] & GetMemMask(n) && 1);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 1;
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxl;
    if (BitLen > bf.BitLen)
        maxl = BitLen;
    else
        maxl = bf.BitLen;
    TBitField res(maxl); //массив из 0

    int minMem;
    if (MemLen < bf.MemLen)
        minMem = MemLen;
    else
        minMem = bf.MemLen;

    for (int i = 0; i < minMem; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = minMem; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMem; i < bf.MemLen; i++)
            res.pMem[i] = bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxl;
    if (BitLen > bf.BitLen)
        maxl = BitLen;
    else
        maxl = bf.BitLen;
    TBitField res(maxl); //массив из 0

    int minMem;
    if (MemLen < bf.MemLen)
        minMem = MemLen;
    else
        minMem = bf.MemLen;

    for (int i = 0; i < minMem; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (!GetBit(i))
            res.SetBit(i);
    }
    
    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char s;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> s;
        if (s == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
