#include "vector2.h"

Vector2::Vector2(double x, double y) : x(x), y(y) {}

Vector2::Vector2(const Vector2 &v) : x(v.x), y(v.y) {}

Vector2 &Vector2::operator=(const Vector2 &v)
{
    x = v.x;
    y = v.y;
    return *this;
}

Vector2 Vector2::operator+(const Vector2 &v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 &Vector2::operator+=(const Vector2 &v)
{
    *this = *this + v;
    return *this;
}

Vector2 Vector2::operator*(const double lambda) const
{
    return Vector2(lambda * x, lambda * y);
}

Vector2 Vector2::operator-(const Vector2 &v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 &Vector2::rotate(const double rad)
{
    *this = Vector2(
        x * cos(rad) - y * sin(rad),
        x * sin(rad) + y * cos(rad));
    return *this;
}

double Vector2::abs() const
{
    // return sqrt(abssq()); de inline gyorsabb lesz
    return sqrt(x * x + y * y);
}

double Vector2::abssq() const
{
    return x * x + y * y;
}

Vector2 &Vector2::normalize()
{
    double len = abs();
    if (len != 0)
    {
        x /= len;
        y /= len;
    }
    return *this;
}

double Vector2::PointSegDist(const Vector2 &a, const Vector2 &b, const Vector2 p, bool &perp, Vector2 &closest)
{
    // https://www.youtube.com/watch?v=egmZJU-1zPU
    Vector2 ab = b - a;
    Vector2 ap = p - a;
    double skalar = DotProduct(ab, ap);
    double lensq = ab.abssq();
    // basically hogy hol van a vonalon, aránylag
    double d =  skalar / lensq;

    // magyarán ha csak azok érdekelnek, akik
    if (d <= 0)
    {
        closest = a;
        perp = false;
    }
    else if (d >= 1)
    {
        closest = b;
        perp = false;
    }
    else
    {
        closest = a + ab * d;
        perp = true;
    }

    return (p - closest).abs();
}

double Vector2::DotProduct(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}

bool Vector2::operator==(const Vector2 &v) const
{
    return x == v.x && y == v.y;
}

bool Vector2::operator!=(const Vector2 &v) const
{
    return !(*this == v);
}
