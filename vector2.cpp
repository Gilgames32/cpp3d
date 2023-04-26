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
    return sqrt(x * x + y * y);
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
