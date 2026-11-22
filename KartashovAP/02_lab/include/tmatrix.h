// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <algorithm>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
    size_t sz;
    T* pMem;
public:
    TDynamicVector(size_t size = 1) : sz(size)
    {
        if (sz == 0)
            throw out_of_range("Vector size should be greater than zero");
        if (sz > MAX_VECTOR_SIZE)
            throw length_error("Vector size is too large");
        pMem = new T[sz](); // У типа T д.б. конструктор по умолчанию
    }

    TDynamicVector(T* arr, size_t s) : sz(s)
    {
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
        if (sz == 0)
            throw out_of_range("Vector size should be greater than zero");
        if (sz > MAX_VECTOR_SIZE)
            throw length_error("Vector size is too large");
        pMem = new T[sz];
        std::copy(arr, arr + sz, pMem);
    }

    TDynamicVector(const TDynamicVector& v) : sz(v.sz)
    {
        pMem = new T[sz];
        std::copy(v.pMem, v.pMem + sz, pMem);
    }

    TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem)
    {
        v.sz = 0;
        v.pMem = nullptr;
    }

    ~TDynamicVector()
    {
        delete[] pMem;
    }

    TDynamicVector& operator=(const TDynamicVector& v)
    {
        if (this == &v)
            return *this;
        if (sz != v.sz)
        {
            T* tmp = new T[v.sz];
            delete[] pMem;
            sz = v.sz;
            pMem = tmp;
        }
        std::copy(v.pMem, v.pMem + sz, pMem);
        return *this;
    }

    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        if (this == &v)
            return *this;
        delete[] pMem;
        sz = v.sz;
        pMem = v.pMem;
        v.sz = 0;
        v.pMem = nullptr;
        return *this;
    }

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[](size_t ind)
    {
        return pMem[ind];
    }
    const T& operator[](size_t ind) const
    {
        return pMem[ind];
    }

    // индексация с контролем
    T& at(size_t ind)
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }
    const T& at(size_t ind) const
    {
        if (ind >= sz)
            throw out_of_range("Index out of range");
        return pMem[ind];
    }

    // сравнение
    bool operator==(const TDynamicVector& v) const noexcept
    {
        if (sz != v.sz)
            return false;
        for (size_t i = 0; i < sz; i++)
            if (pMem[i] != v.pMem[i])
                return false;
        return true;
    }
    bool operator!=(const TDynamicVector& v) const noexcept
    {
        return !(*this == v);
    }

    // скалярные операции
    TDynamicVector operator+(T val)
    {
        TDynamicVector res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] + val;
        return res;
    }
    TDynamicVector operator-(T val)
    {
        TDynamicVector res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] - val;
        return res;
    }
    TDynamicVector operator*(T val)
    {
        TDynamicVector res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] * val;
        return res;
    }

    // векторные операции
    TDynamicVector operator+(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw invalid_argument("Vector sizes must match");
        TDynamicVector res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] + v.pMem[i];
        return res;
    }
    TDynamicVector operator-(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw invalid_argument("Vector sizes must match");
        TDynamicVector res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] - v.pMem[i];
        return res;
    }
    T operator*(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw invalid_argument("Vector sizes must match");
        T res = T();
        for (size_t i = 0; i < sz; i++)
            res += pMem[i] * v.pMem[i];
        return res;
    }

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            istr >> v.pMem[i]; // требуется оператор>> для типа T
        return istr;
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
        return ostr;
    }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        if (sz > MAX_MATRIX_SIZE)
            throw length_error("Matrix size is too large");
        for (size_t i = 0; i < sz; i++)
            pMem[i] = TDynamicVector<T>(sz);
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];
    using TDynamicVector<TDynamicVector<T>>::size;
    using TDynamicVector<TDynamicVector<T>>::at;

    // сравнение
    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return TDynamicVector<TDynamicVector<T>>::operator==(m);
    }
    bool operator!=(const TDynamicMatrix& m) const noexcept
    {
        return !(*this == m);
    }

    // матрично-скалярные операции
    TDynamicMatrix operator*(const T& val)
    {
        TDynamicMatrix res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] * val;
        return res;
    }

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        if (sz != v.size())
            throw invalid_argument("Matrix and vector sizes must match");
        TDynamicVector<T> res(sz);
        for (size_t i = 0; i < sz; i++)
            res[i] = pMem[i] * v;
        return res;
    }

    // матрично-матричные операции
    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw invalid_argument("Matrix sizes must match");
        TDynamicMatrix res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] + m.pMem[i];
        return res;
    }
    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw invalid_argument("Matrix sizes must match");
        TDynamicMatrix res(sz);
        for (size_t i = 0; i < sz; i++)
            res.pMem[i] = pMem[i] - m.pMem[i];
        return res;
    }
    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        if (sz != m.sz)
            throw invalid_argument("Matrix sizes must match");
        TDynamicMatrix res(sz);
        for (size_t i = 0; i < sz; i++)
            for (size_t k = 0; k < sz; k++)
                for (size_t j = 0; j < sz; j++)
                    res.pMem[i][j] += pMem[i][k] * m.pMem[k][j];
        return res;
    }

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; i++)
            istr >> m.pMem[i];
        return istr;
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; i++)
            ostr << m.pMem[i] << '\n';
        return ostr;
    }
};

#endif
