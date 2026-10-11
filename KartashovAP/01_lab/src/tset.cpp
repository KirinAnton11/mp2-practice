// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <algorithm>
#include <stdexcept>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp < 0)
    {
        throw std::invalid_argument("Negative max power");
    }
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
    {
        throw std::out_of_range("Element out of universe range");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
    {
        throw std::out_of_range("Element out of universe range");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
    {
        throw std::out_of_range("Element out of universe range");
    }
    BitField.ClrBit(Elem);
}

const TSet& TSet::operator=(const TSet& s)
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const
{
    if (MaxPower != s.MaxPower)
    {
        return 0;
    }
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const
{
    return (*this == s) ? 0 : 1;
}

TSet TSet::operator+(const TSet& s)
{
    TSet res(std::max(MaxPower, s.MaxPower));
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
    {
        throw std::out_of_range("Element out of universe range");
    }
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
    {
        throw std::out_of_range("Element out of universe range");
    }
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet& s)
{
    TSet res(std::max(MaxPower, s.MaxPower));
    res.BitField = BitField & s.BitField;
    return res;
}

TSet TSet::operator~(void)
{
    TSet res(MaxPower);
    res.BitField = ~BitField;
    return res;
}

istream& operator>>(istream& istr, TSet& s)
{
    int elem;
    char ch;
    istr >> ch;
    while (istr >> elem)
    {
        s.InsElem(elem);
        istr >> ch;
        if (ch == '}')
        {
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.MaxPower; ++i)
    {
        if (s.IsMember(i))
        {
            if (!first)
            {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}