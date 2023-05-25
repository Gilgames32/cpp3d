#if !defined(VECTOR2_H)
#define VECTOR2_H

#include "iostream"
#include "math.h"

#include "memtrace.h"

class Vector2
{
public:
    double x, y; /**< x és y koordináták */

    /**
     * alapértelmezett konstruktor
     * paraméterek hiányában nullvektor jön létre
     * @param x x koordináta
     * @param y y koordináta
     */
    Vector2(double x = 0, double y = 0);

    /**
     * másoló konstruktor
     */
    Vector2(const Vector2 &v);

    /**
     * értékadó operátor
     */
    Vector2 &operator=(const Vector2 &v);

    /**
     * összeadó operátor
     * @return vektorok összege
     */
    Vector2 operator+(const Vector2 &v) const;

    /**
     * összeadó-értékadó operátor
     */
    Vector2 &operator+=(const Vector2 &v);

    /**
     * szorzás operátor (nyújtás)
     * @param lambda nyújtás mértéke
     * @return vektor lambdaszorosa
     */
    Vector2 operator*(const double lambda) const;

    /**
     * kivonás operátor
     * @return vektorok különbsége
     */
    Vector2 operator-(const Vector2 &v) const;

    /**
     * haszonlító operátor (egyenlő)
     */
    bool operator==(const Vector2 &v) const;

    /**
     * hasonlító operátor (nem egyenlő)
     */
    bool operator!=(const Vector2 &v) const;

    /**
     * vektor elforgatása (önmagát)
     * @param rad forgatás mértéke radiánban
     */
    Vector2 &rotate(const double rad);

    /**
     * vektor hossza (abszolút értéke)
     */
    double abs() const;

    /**
     * vekotr hosszának négyzete (nincs gyök, gyorsabb)
     */
    double abssq() const;

    /**
     * vektor normálisa
     * @return megegyező írányú, egységnyi hosszúságú vektor
     */
    Vector2 normalize() const;

    /**
     * pont és szakasz távolsága
     * @param seg1 szakasz egyik végpontjának koordinátája
     * @param seg2 szakasz másik végpontjának koordinátája
     * @param p pont koordinátája
     * @param perp referencia, igazra állítja, ha a merőleges vetület a szakaszon belül van
     * @param closest referencia, a szakasz a ponthoz legközelebbi pontjára állítja
     * @return pont és szakasz távolsága
     */
    static double PointSegDist(const Vector2 &seg1, const Vector2 &seg2, const Vector2 p, bool &perp, Vector2 &closest);

    /**
     * skaláris szorzat
     * @param a egyik vektor
     * @param b másik vektor
     * @return skalis szorzat
     */
    static double DotProduct(const Vector2 &a, const Vector2 &b);
};

/**
 * ugyan olyan típusok párja
 */
template <typename T>
class Duo
{
public:
    T x, y;
    Duo(T x0 = 0, T y0 = 0) : x(x0), y(y0) {}
};

/**
 * különböző típusok párja
 */
template <typename A, typename B>
class Pair
{
public:
    A a;
    B b;
    Pair(A a0 = 0, B b0 = 0) : a(a0), b(b0) {}
};

#endif