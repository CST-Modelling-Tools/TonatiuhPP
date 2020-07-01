#pragma once

#include <QVector>

/**
 * Matrix2D stores a 2D matrix
 * Essentially, it is a QVector class with a modified indexing
 * The matrix indices starts from 0
 *
 * Usage:
 * Matrix2D<double> m(3, 5);
 * m.fill(0.);
 * m(2, 3) = 1.;
 */

template <class T>
class Matrix2D
{
public:
    // constructors
    Matrix2D() {}
    Matrix2D(int rows, int cols);

    // components
    int rows() const {return m_rows;}
    int cols() const {return m_cols;}
    void resize(int rows, int cols);

    QVector<T>& data() {return m_data;}
    const QVector<T>& data() const {return m_data;}
    void setData(const QVector<T>& data) {m_data = data;}

    int index(int r, int c) const {return r*m_cols + c;}
    T& operator()(int r, int c) {return m_data[index(r, c)];}
    const T& operator()(int r, int c) const {return m_data[index(r, c)];}

    // functions
    void fill(const T& value) {m_data.fill(value);}

    Matrix2D<T>& operator+=(const Matrix2D<T>& other);
    bool operator==(const Matrix2D<T>& other) const;

protected:
    int m_rows;
    int m_cols;
    QVector<T> m_data;
};



template<class T>
Matrix2D<T>::Matrix2D(int rows, int cols)
{
    resize(rows, cols);
}

template<class T>
void Matrix2D<T>::resize(int rows, int cols)
{
    Q_ASSERT(rows > 0);
    Q_ASSERT(cols > 0);
    m_rows = rows;
    m_cols = cols;
    m_data.resize(rows*cols);
}

template<class T>
Matrix2D<T>& Matrix2D<T>::operator+=(const Matrix2D<T>& other)
{
    Q_ASSERT(m_rows == other.m_rows);
    Q_ASSERT(m_cols == other.m_cols);
    for (int n = 0; n < m_data.size(); ++n)
        m_data[n] += other.m_data[n];
    return *this;
}

template<class T>
bool Matrix2D<T>::operator==(const Matrix2D<T>& other) const
{
    if (m_rows != other.m_rows) return false;
    if (m_cols != other.m_cols) return false;
    if (m_data != other.m_data) return false;
    return true;
}
