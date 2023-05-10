#ifndef VECTOR2_H
#define VECTOR2_H

#include "math.h"
#include <iostream>

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
    bool operator==(const Vector2 &v) const;
    bool operator!=(const Vector2 &v) const;

    // rotates SELF
    Vector2 &rotate(const double rad);

    double abs() const;
    double abssq() const;

    // returns the same vector but 1 unit long
    Vector2 normalize() const;
    //~Vector2();

    // pont és szakasz távolsága
    static double PointSegDist(const Vector2& seg1, const Vector2& seg2, const Vector2 p, bool &perp, Vector2 &closest);

    // skaláris szorzat
    static double DotProduct(const Vector2& a, const Vector2& b);
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);

// same type but handled together
template <typename T>
class Duo
{
private:
    /* data */
public:
    T x, y;
    Duo(T x0 = 0, T y0 = 0)  : x(x0), y(y0) {}
};

// different types handled together
template <typename A, typename B>
class Pair
{
private:
    /* data */
public:
    A a;
    B b;
    Pair(A a0 = 0, B b0 = 0) : a(a0), b(b0) {}
};

#endif