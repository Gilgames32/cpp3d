#ifndef VECTOR2_H
#define VECTOR2_H

class vector2
{
private:
    /* data */
public:
    double x, y;
    vector2(double x, double y);
    vector2 operator+(const vector2 v);
    vector2 operator+=(const vector2& v);
    vector2 operator*(const double lambda);
    vector2 rotate(double rad);
};






#endif VECTOR2_H