#include "matrix.h"


Matrix::Matrix(int mxsizex, int mxsizey, int** mxgrid) : sizex(mxsizex), sizey(mxsizey), grid(nullptr)
{
    if (sizex != 0)
    {
        grid = new int*[sizex];
        for (int s = 0; s < sizex; s++)
        {
            grid[s] = new int[sizey];
            for (int o = 0; o < sizey; o++)
            {
                grid[s][o] = mxgrid[s][o];
            }
        }
    }
}

Matrix::Matrix(const Matrix &mx) : sizex(mx.sizex), sizey(mx.sizey) {
    if (sizex != 0)
    {
        grid = new int*[sizex];
        for (int s = 0; s < sizex; s++)
        {
            grid[s] = new int[sizey];
            for (int o = 0; o < sizey; o++)
            {
                grid[s][o] = mx.grid[s][o];
            }
        }
    }
}

void Matrix::operator=(const Matrix &mx)
{
    // TODO!!! delete old
    for (int i = 0; i < sizex; i++)
        delete[] grid[i];
    delete[] grid;

    sizex = mx.sizex;
    sizey = mx.sizey;
    grid = new int*[sizex];
    for (int s = 0; s < sizex; s++)
    {
        grid[s] = new int[sizey];
        for (int o = 0; o < sizey; o++)
        {
            grid[s][o] = mx.grid[s][o];
        }
    }
}

Matrix::~Matrix(){
    for (int i = 0; i < sizex; i++)
        delete[] grid[i];
        
    delete[] grid;
}

int* Matrix::operator[](size_t index) const{
    return grid[index];
}