#include "matrix.h"

const Duo<size_t> &Matrix::GetSize() const { return size; }

Matrix::Matrix(int mxsizex, int mxsizey, int **mxgrid) : size(Duo<size_t>(mxsizex, mxsizey)), grid(nullptr)
{
    grid = new int *[size.x];
    for (size_t s = 0; s < size.x; s++)
    {
        grid[s] = new int[size.y];
        for (size_t o = 0; o < size.y; o++)
        {
            grid[s][o] = mxgrid[s][o];
        }
    }
}

Matrix::Matrix(const Matrix &mx) : size(mx.GetSize())
{
    if (size.x != 0)
    {
        grid = new int *[size.x];
        for (size_t s = 0; s < size.x; s++)
        {
            grid[s] = new int[size.y];
            for (size_t o = 0; o < size.y; o++)
            {
                grid[s][o] = mx.grid[s][o];
            }
        }
    }
}

void Matrix::operator=(const Matrix &mx)
{
    for (size_t i = 0; i < size.x; i++)
        delete[] grid[i];
    delete[] grid;

    size = mx.GetSize();
    grid = new int *[size.x];
    for (size_t s = 0; s < size.x; s++)
    {
        grid[s] = new int[size.y];
        for (size_t o = 0; o < size.y; o++)
        {
            grid[s][o] = mx.grid[s][o];
        }
    }
}

Matrix::~Matrix()
{
    for (size_t i = 0; i < size.x; i++)
        delete[] grid[i];

    delete[] grid;
}

// good luck oob tesztelni ha már int tömb lol
int *Matrix::operator[](size_t index) const
{
    if (index < 0 || index >= size.x)
        throw std::out_of_range("Nem létező oszlop");
    
    return grid[index];
}