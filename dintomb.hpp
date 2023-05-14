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

// kettős indirekcijó jajdejó
template <typename T>
class DinTomb
{
private:
    T **data;
    size_t size;

public:
    class DTIterator;
    class DTIteratorConst;

    DinTomb(size_t s = 0) : size(s) { data = new T *[size]; }

    ~DinTomb()
    {
        for (size_t i = 0; i < size; i++)
        {
            delete data[i];
        }
        delete[] data;
    }

    size_t Size() const { return size; }

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

    void Delete(int index)
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("alul vagy tul van idexelve halo");

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

    T &operator[](int index)
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("alul vagy tul van idexelve halo");

        return *(data[index]);
    }

    const T &operator[](int index) const
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("alul vagy tul van idexelve halo");

        return *(data[index]);
    }

    DTIterator begin() { return DTIterator(*this); }

    DTIterator end() { return DTIterator(*this, size); }

    DTIteratorConst begin() const { return DTIteratorConst(*this); }

    DTIteratorConst end() const { return DTIteratorConst(*this, size); }

    class DTIterator
    {
        DinTomb *parent;
        size_t idx;

    public:
        DTIterator() : parent(0), idx(0) {}
        DTIterator(DinTomb &t, size_t offset = 0) : parent(&t), idx(offset) {}

        DTIterator &operator++()
        {
            if (idx != parent->size)
                ++idx;
            return *this;
        }

        DTIterator operator++(int)
        {
            DTIterator tmp = *this;
            operator++();
            return tmp;
        }

        bool operator!=(const DTIterator &i) const { return (idx != i.idx); }

        bool operator==(const DTIterator &i) const { return !operator!=(i); }

        T &operator*() const
        {
            if (idx != parent->size)
                return *(parent->data[idx]);
            else
                throw std::out_of_range("utolso elem lesz ez helo");
        }

        T *operator->() const { return &operator*(); }

        size_t Index() { return idx; }
    };

    class DTIteratorConst : public DTIterator
    {
    public:
        DTIteratorConst() {}
        DTIteratorConst(const DinTomb &a, size_t ix = 0) : DTIterator(const_cast<DinTomb &>(a), ix) {}
        const T &operator*() const { return DTIterator::operator*(); }
        const T *operator->() const { return &operator*(); }
    };
};

#endif // DINTOMB_H
