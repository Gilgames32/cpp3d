#include "vector2.h"

Vector2::Vector2(double x = 0, double y = 0) : x(x), y(y) {}

Vector2::Vector2(const Vector2 &v) : x(v.x), y(v.y) {}

Vector2& Vector2::operator=(const Vector2 &v){
    x = v.x; y = v.y;
    return *this;
}

Vector2 Vector2::operator+(const Vector2 &v){
    return Vector2(x + v.x, y + v.y);
}

Vector2& Vector2::operator+=(const Vector2 &v){
    *this = *this + v;
    return *this;
}

Vector2 Vector2::operator*(const double lambda){
    return Vector2(lambda * x, lambda * y);
}

Vector2& Vector2::rotate(const double rad){
    *this = Vector2(
        x * cos(rad) - y * sin(rad),
        x * sin(rad) + y * cos(rad)
    );
    return *this;
}