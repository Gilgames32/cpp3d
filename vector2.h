#ifndef VECTOR2_H
#define VECTOR2_H

#include "math.h"

class Vector2
{
public:
    double x, y;
    Vector2(double x = 0, double y = 0);
    Vector2(const Vector2 &v);

    Vector2 &operator=(const Vector2 &v);
    Vector2 operator+(const Vector2 &v) const;
    Vector2 &operator+=(const Vector2 &v);
    Vector2 operator*(const double lambda) const;

    Vector2 operator-(const Vector2 &v) const;

    // rotates SELF
    Vector2 &rotate(const double rad);

    double abs() const;

    // returns the same vector but 1 unit long
    Vector2 &normalize();
    //~Vector2();
};

// same type but handled together
template <typename T>
class duo
{
private:
    /* data */
public:
    T x, y;
    duo(T x0 = 0, T y0 = 0) : x(x0), y(y0) {}
};

// different types handled together
template <typename A, typename B>
class pair
{
private:
    /* data */
public:
    A a;
    B b;
    pair(A a0 = 0, B b0 = 0) : a(a0), b(b0) {}
};

#endif