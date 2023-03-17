#ifndef VECTOR2_H
#define VECTOR2_H

class vector2
{
private:
    /* data */
public:
    double x, y;
    vector2(double setx = 0, double sety = 0){
        x = setx;
        y = sety;
    }

    vector2 operator+(const vector2& v){
        return vector2(x + v.x, y + v.y);
    }

    vector2& operator+=(const vector2& v){
        *this = this->operator+(v);
        return *this;
    }

    vector2 operator*(const double lambda)
    {
        return vector2(x*lambda, y*lambda);
    }

    void rotate(const double rad){
        *this = vector2(
            x * cos(rad) - y * sin(rad),
            x * sin(rad) + y * cos(rad)
        );
    }

};

vector2 rotate(vector2 v, const double rad){
    v.rotate(rad);
    return v;
}


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