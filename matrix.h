#if !defined(MATRIX_H)
#define MATRIX_H

#include <iostream>
#include <fstream>

class Level
{
public:
    int sizex, sizey;
    int **grid;

    Level(const char *fileName);
    Level(const Level &);
    ~Level();

    int *operator[](size_t index) const;
};

#endif // MATRIX_H
