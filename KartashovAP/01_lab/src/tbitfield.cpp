// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>
#include <stdexcept>

static const int BITS_IN_TELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len) : BitLen(len), pMem(nullptr), MemLen(0)
{
    if (len < 0)
    {
        throw std::invalid_argument("Negative length");
    }

    MemLen = (BitLen + BITS_IN_TELEM - 1) / BITS_IN_TELEM;

    if (MemLen > 0)
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), pMem(nullptr), MemLen(bf.MemLen)
{
    if (MemLen > 0)
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen)
    {
        throw std::out_of_range("Bit index out of range");
    }
    return n / BITS_IN_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen)
    {
        throw std::out_of_range("Bit index out of range");
    }
    return 1u << (n % BITS_IN_TELEM);
}

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
    {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
    {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
    {
        throw std::out_of_range("Bit index out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0 ? 1 : 0;
}

const TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this != &bf)
    {
        if (MemLen != bf.MemLen)
        {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = (MemLen > 0) ? new TELEM[MemLen] : nullptr;
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
    {
        return 0;
    }
    for (int i = 0; i < MemLen; ++i)
    {
        if (pMem[i] != bf.pMem[i])
        {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return (*this == bf) ? 0 : 1;
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int maxLen = std::max(BitLen, bf.BitLen);
    TBitField res(maxLen);

    int minMem = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minMem; ++i)
    {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen)
    {
        for (int i = minMem; i < MemLen; ++i)
        {
            res.pMem[i] = pMem[i];
        }
    }
    else
    {
        for (int i = minMem; i < bf.MemLen; ++i)
        {
            res.pMem[i] = bf.pMem[i];
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField& bf)
{
    int maxLen = std::max(BitLen, bf.BitLen);
    TBitField res(maxLen);

    int minMem = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minMem; ++i)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; ++i)
    {
        res.pMem[i] = ~pMem[i];
    }

    int extraBits = BitLen % BITS_IN_TELEM;
    if (extraBits != 0 && MemLen > 0)
    {
        TELEM mask = (1u << extraBits) - 1u;
        res.pMem[MemLen - 1] &= mask;
    }

    return res;
}

istream& operator>>(istream& istr, TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; ++i)
    {
        char ch;
        istr >> ch;
        if (ch == '1')
        {
            bf.SetBit(i);
        }
        else if (ch == '0')
        {
            bf.ClrBit(i);
        }
        else
        {
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; ++i)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}