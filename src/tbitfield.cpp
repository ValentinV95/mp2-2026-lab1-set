// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


#define SIZE_TELEM_BIT (sizeof(TELEM) * 8)

TBitField::TBitField(int len) : BitLen{ len }
{
    if (len <= 0) {
        throw out_of_range("BitLen must be a positive");
    }
    MemLen = (BitLen + SIZE_TELEM_BIT - 1) / SIZE_TELEM_BIT;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) : pMem{ new TELEM[bf.MemLen] }, BitLen{ bf.BitLen }, MemLen{ bf.MemLen } // конструктор копирования
{
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / SIZE_TELEM_BIT;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return static_cast<TELEM>(1) << (n % SIZE_TELEM_BIT);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw out_of_range("Bit index out of range");
    }
    TELEM mask = GetMemMask(n);
    int index = GetMemIndex(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw out_of_range("Bit index out of range");
    }
    TELEM mask = GetMemMask(n);
    int index = GetMemIndex(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw out_of_range("Bit index out of range");
    }
    TELEM mask = GetMemMask(n);
    int index = GetMemIndex(n);
    return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    TBitField temp(bf);
    this->Swap(temp);
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return false;
    }
    for (int i = 0; i < BitLen; ++i) {
        if (GetBit(i) != bf.GetBit(i)) {
            return false;
        }
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) const // операция "или"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField res(maxBitLen);

    for (int i = 0; i < maxBitLen; ++i) {
        bool bit1 = (i < BitLen) ? GetBit(i) : false;
        bool bit2 = (i < bf.BitLen) ? bf.GetBit(i) : false;
        if (bit1 || bit2) {
            res.SetBit(i);
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) const // операция "и"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField res(maxBitLen);

    for (int i = 0; i < maxBitLen; ++i) {
        bool bit1 = (i < BitLen) ? GetBit(i) : false;
        bool bit2 = (i < bf.BitLen) ? bf.GetBit(i) : false;
        if (bit1 && bit2) {
            res.SetBit(i);
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < BitLen; ++i) {
        if (!GetBit(i)) {
            res.SetBit(i);
        }
    }
    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string input;
    istr >> input;
    for (int i = 0; i < input.size() && i < bf.GetLength(); i++) {
        if (input[i] == '1') {
            bf.SetBit(i);
        }
        else if (input[i] == '0') {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); ++i) {
        ostr << (bf.GetBit(i) ? '1' : '0');
    }
    return ostr;
}

void TBitField::Swap(TBitField& bf) noexcept
{
    swap(pMem, bf.pMem);
    swap(BitLen, bf.BitLen);
    swap(MemLen, bf.MemLen);
}
