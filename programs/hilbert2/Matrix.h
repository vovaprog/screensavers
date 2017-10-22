#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>

template<int RowCount, int ColCount>
struct Matrix
{
    Matrix()
    {
        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                m[i][j] = 0;
            }
        }
    }

    Matrix(int (&init)[RowCount][ColCount])
    {
        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                m[i][j] = init[i][j];
            }
        }
    }

    Matrix<RowCount, ColCount>& operator += (const Matrix<RowCount, ColCount> &mat)
    {
        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                m[i][j] += mat.m[i][j];
            }
        }
        return *this;
    }

    Matrix<RowCount, ColCount> operator - (const Matrix<RowCount, ColCount> &mat) const
    {
        Matrix<RowCount, ColCount> res;

        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                res.m[i][j] = m[i][j] - mat.m[i][j];
            }
        }
        return res;
    }

    bool operator == (const Matrix<RowCount, ColCount> &mat) const
    {
        Matrix<RowCount, ColCount> res;

        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                if (m[i][j] != mat.m[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator != (const Matrix<RowCount, ColCount> &mat) const
    {
        return !(*this == mat);
    }

    void print() const
    {
        std::cout << "==========" << std::endl;
        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < ColCount; ++j)
            {
                std::cout << m[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "==========" << std::endl;
    }

    int m[RowCount][ColCount];
};


template<int RowCount1, int CommonCount, int ColCount2>
Matrix<RowCount1, ColCount2> mul(const Matrix<RowCount1, CommonCount> &m1, const Matrix<CommonCount, ColCount2> &m2)
{
    Matrix<RowCount1, ColCount2> res;

    for (int ri = 0; ri < RowCount1; ++ri)
    {
        for (int rj = 0; rj < ColCount2; ++rj)
        {
            res.m[ri][rj] = 0;

            for (int k = 0; k < CommonCount; ++k)
            {
                res.m[ri][rj] += m1.m[ri][k] * m2.m[k][rj];
            }
        }
    }

    return res;
}

template<int MatSize, int SourceCols>
Matrix<MatSize, SourceCols> rotate(
    const Matrix<MatSize, SourceCols> &m,
    const Matrix<MatSize, MatSize> &rot)
{
    return mul(rot, m);
}

#endif
