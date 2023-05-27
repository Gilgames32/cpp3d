#if !defined(DINTOMB_H)
#define DINTOMB_H

#include "iostream"

#include "memtrace.h"

/*
<rant>
    when cpp gives you std
    dont use vector
    make gdb crash because of push_back()
    GET MAD!
    i dont want your damn vector
    what the hell am i supposed to do with these?
    demand to see prog2's tárgyfelelős
    make g++ rue the day it thought it could build my code
    do you know who i am?
    im the man whos gunna spend 120 hours on a telefonkönyv
    with SDL
    im gonna get my fingers to write a code that burns your pc down
</rant>
*/

/**
 * kettős indirekcióval tárol dinamikusan foglalt memóriaterületen adott típusú változókat
 */
template <typename T>
class DinTomb
{
private:
    T **data;    /**< adott típusú adatokra mutató pointerek tömbje */
    size_t size; /**< tömb méreteF */

public:
    class DTIterator;      /**< iterátor osztály */
    class DTIteratorConst; /**< konstans iterátor osztály */

    /**
     * alapértelmezett konstruktor
     * @param s méret
     */
    DinTomb(size_t s = 0) : size(s) { data = new T *[size]; }

    /**
     * destruktor
     */
    ~DinTomb()
    {
        for (size_t i = 0; i < size; i++)
        {
            delete data[i];
        }
        delete[] data;
    }

    /**
     * méret getter
     * @return méret
     */
    size_t Size() const { return size; }

    /**
     * hozzáfűzés
     * a tömb méretét eggyel növeli, utolsó elem új lesz
     * @param add hozzáfűzendő elem
     */
    void Append(const T &add)
    {
        T **temp = new T *[size + 1];
        for (size_t i = 0; i < size; i++)
        {
            temp[i] = data[i];
        }
        temp[size] = new T(add);
        size++;
        delete[] data;
        data = temp;
    }

    /**
     * törlés
     * adott indexű elemet felszabadít, eggyel csökken a méret
     * @param index törölni kívánt index eleme
     * @throw tartományon kívül eső index
     */
    void Delete(size_t index)
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Tartományon kívül eső index");

        T **temp = new T *[size - 1];
        for (size_t i = 0; i < size; i++)
        {
            if (i < index)
                temp[i] = data[i];
            else
                temp[i] = data[i + 1];
        }
        delete data[index];
        size--;
        delete[] data;
        data = temp;
    }

    /**
     * indexelő operátor
     * @param index index
     * @return referencia az indexelt elemre
     * @throw tartományon kívül eső index
     */
    T &operator[](size_t index)
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Tartományon kívül eső index");

        return *(data[index]);
    }

    /**
     * konstans indexelő operátor
     * @param index index
     * @return konstans referencia az indexelt elemre
     * @throw tartományon kívül eső index
     */
    const T &operator[](size_t index) const
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Tartományon kívül eső index");

        return *(data[index]);
    }

    /**
     * @return tömb első elemére mutató iterátor
     */
    DTIterator begin() { return DTIterator(*this); }

    /**
     * @return tömb utolsó utáni elemére mutató iterátor
     */
    DTIterator end() { return DTIterator(*this, size); }

    /**
     * @return tömb első elemére mutató konstans iterátor
     */
    DTIteratorConst begin() const { return DTIteratorConst(*this); }

    /**
     * @return tömb utolsó utáni elemére mutató konstans iterátor
     */
    DTIteratorConst end() const { return DTIteratorConst(*this, size); }

    class DTIterator
    {
        DinTomb *parent; /**< iterátorhoz tartozó tömb első eleme */
        size_t idx;      /**< index */

    public:
        /**
         * alapértelmezett konstruktor
         */
        DTIterator() : parent(0), idx(0) {}

        /**
         * konstrukotr
         * @param t hozzá tartozó tömb
         * @param offset kezdeti index
         */
        DTIterator(DinTomb &t, size_t offset = 0) : parent(&t), idx(offset) {}

        /**
         * pre-inkrementáló operátor
         * @return referencia a megnövelt iterátorra
         */
        DTIterator &operator++()
        {
            if (idx != parent->size)
                ++idx;
            return *this;
        }

        /**
         * poszt-inkrementáló operátor
         * @return még meg nem növelt iterátor
         */
        DTIterator operator++(int)
        {
            DTIterator tmp = *this;
            operator++();
            return tmp;
        }

        /**
         * hasonlító operátor (nem egyenlő)
         * @return eredmény
         */
        bool operator!=(const DTIterator &i) const { return (idx != i.idx); }

        /**
         * hasonlító operátor (egyenlő)
         * @return eredmény
         */
        bool operator==(const DTIterator &i) const { return !operator!=(i); }

        /**
         * dereferáló operátor
         * @return referencia az adott elemre
         * @throw utolsó utáni elem dereferálása
         */
        T &operator*() const
        {
            if (idx != parent->size)
                return *(parent->data[idx]);
            else
                throw std::out_of_range("Utolsó utáni elem nem dereferálható");
        }

        /**
         * nyíl operátor
         * @return pointer az adott elemre
         */
        T *operator->() const { return &operator*(); }

        /**
         * @return index
         */
        size_t Index() { return idx; }
    };

    class DTIteratorConst : public DTIterator
    {
    public:
        /**
         * alapértelmezett konstrukor
         */
        DTIteratorConst() {}

        /**
         * konstruktor
         * @param a hozzá tartozó tömb
         * @param ix kezdeti index
         */
        DTIteratorConst(const DinTomb &a, size_t ix = 0) : DTIterator(const_cast<DinTomb &>(a), ix) {}

        /**
         * dereferáló operátor konstans változata
         * @return konstans referencia az adott elemre
         */
        const T &operator*() const { return DTIterator::operator*(); }

        /**
         * nyíl operátor konstans változata
         * @return konstans pointer az adott elemre
         */
        const T *operator->() const { return &operator*(); }
    };
};

#endif // DINTOMB_H
