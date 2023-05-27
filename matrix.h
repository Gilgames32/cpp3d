#if !defined(MATRIX_H)
#define MATRIX_H

#include "iostream"
#include "fstream"

#include "vector2.h"

#include "memtrace.h"

class Matrix
{
private:
    Duo<size_t> size; /**< méret */
    int **grid;       /**< cellaértékeket tartalmazó 2 dimenziós tömb */

public:
    /**
     * alapértelmezett konstruktor
     * @param sizex sorméret
     * @param sizey oszlopméret
     * @param gird tömb pointere
     */
    Matrix(int sizex = 0, int sizey = 0, int **grid = 0);

    /**
     * másoló konstrukotr
     */
    Matrix(const Matrix &);

    /**
     * destrukotr
     */
    ~Matrix();

    /**
     * értékadó operátor
     */
    void operator=(const Matrix &);

    /**
     * indexelő operátor
     * @param index sor indexe
     * @return indexedik sor, int tömb
     * @throw mátrixon kívül eső oszlopindex
     */
    int *operator[](size_t index) const;

    /**
     * méret gettere
     * @return méret
     */
    const Duo<size_t> &GetSize() const;
};

#endif // MATRIX_H
