#if !defined(MATRIX_H)
#define MATRIX_H

#include "iostream"
#include "fstream"

#include "vector2.h"

#include "memtrace.h"

class Matrix
{
private:
    Duo<size_t> size;
    int **grid;

public:
    Matrix(int sizex = 0, int sizey = 0, int **grid = 0);
    Matrix(const Matrix &);
    ~Matrix();

    void operator=(const Matrix &);
    int *operator[](size_t index) const;

    const Duo<size_t> &GetSize() const;
};

#endif // MATRIX_H
