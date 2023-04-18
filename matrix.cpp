#include "matrix.h"


Matrix::Matrix(int sizex, int sizey, int** grid) : sizex(sizex), sizey(sizey), grid(grid) {}

Matrix::Matrix(const Matrix &mx) : sizex(mx.sizex), sizey(mx.sizey) {
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

void Matrix::operator=(const Matrix &mx)
{
    // TODO!!! delete old
    sizex = mx.sizex();
    sizey = mx.sizey();
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