#ifndef VECTOR2_H
#define VECTOR2_H

#include "math.h"

class Vector2
{
public:
    double x, y;
    Vector2(double x = 0, double y = 0);
    Vector2(const Vector2 &v);

    Vector2& operator=(const Vector2 &v);
    Vector2 operator+(const Vector2 &v) const;
    Vector2& operator+=(const Vector2 &v);
    Vector2 operator*(const double lambda);

    Vector2& rotate(const double rad);

    //~Vector2();
};


template <typename T>
class pair
{
private:
    /* data */
public:
    T x, y;
    pair(T x0 = 0, T y0 = 0){
        x = x0;
        y = y0;
    }
};




#endif