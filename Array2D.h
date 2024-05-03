

#ifndef CHECKERS_ARRAY2D_H
#define CHECKERS_ARRAY2D_H

#ifndef ARRAY2D_H_
#define ARRAY2D_H_

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Array
{
    std::vector<T> data;
    void check_range(int idx) const
    {
        if (idx < 0 || static_cast<size_t>(idx) >= data.size())
        {
            throw std::out_of_range("KABOOM: Invalid COLUMN access");
        }
    }
public:
    explicit Array(int size) : data (size)
    {
    }
    T & operator [](int idx)
    {
        check_range(idx);
        return data[idx];
    }
    const T & operator [](int idx) const
    {
        check_range(idx);
        return data[idx];
    }
    void resize(int size)
    {
        data.resize(size);
    }
};

template<typename T>
class Array2D
{
    std::vector<Array<T>> data;
    void check_range(int idx) const
    {
        if (idx < 0 || static_cast<size_t>(idx) >= data.size())
        {
            throw std::out_of_range("KABOOM: Invalid ROW access");
        }
    }
public:
    explicit Array2D (int nrows, int ncols) : data ( nrows, Array<T>(ncols)) {}
    Array<T> & operator [](int idx)
    {
        check_range(idx);
        return data[idx];
    }
    const Array<T> & operator [](int idx) const
    {
        check_range(idx);
        return data[idx];
    }

    void resize(int nrows, int ncols)
    {
        data.resize(nrows, Array<T>(0));
        for (auto & row: data)
            row.resize(ncols);
    }
};


#endif /* ARRAY2D_H_ */




#endif //CHECKERS_ARRAY2D_H
