#if !defined(MATRIX_H)
#define MATRIX_H

#include "iostream"
#include "fstream"

#include "memtrace.h"


class Matrix
{
public:
    int sizex, sizey;
    int **grid;

    Matrix(int sizex = 0, int sizey = 0, int**grid = 0);
    Matrix(const Matrix &);
    void operator=(const Matrix &);
    ~Matrix();

    int *operator[](size_t index) const;
};

#endif // MATRIX_H
