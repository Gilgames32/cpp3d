#include "matrix.h"

const Duo<int> &Matrix::GetSize() const { return size; }

Matrix::Matrix(int mxsizex, int mxsizey, int **mxgrid) : size(Duo<int>(mxsizex, mxsizey)), grid(nullptr)
{
    grid = new int *[size.x];
    for (int s = 0; s < size.x; s++)
    {
        grid[s] = new int[size.y];
        for (int o = 0; o < size.y; o++)
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
        for (int s = 0; s < size.x; s++)
        {
            grid[s] = new int[size.y];
            for (int o = 0; o < size.y; o++)
            {
                grid[s][o] = mx.grid[s][o];
            }
        }
    }
}

void Matrix::operator=(const Matrix &mx)
{
    for (int i = 0; i < size.x; i++)
        delete[] grid[i];
    delete[] grid;

    size = mx.GetSize();
    grid = new int *[size.x];
    for (int s = 0; s < size.x; s++)
    {
        grid[s] = new int[size.y];
        for (int o = 0; o < size.y; o++)
        {
            grid[s][o] = mx.grid[s][o];
        }
    }
}

Matrix::~Matrix()
{
    for (int i = 0; i < size.x; i++)
        delete[] grid[i];

    delete[] grid;
}

int *Matrix::operator[](size_t index) const
{
    return grid[index];
}