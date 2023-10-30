#ifndef QUEUE_H
#define QUEUE_H
#include <QDebug>
template<class T>
class queue {
private:
    int size, hh, tt;
    T* a;
public:
    queue(int size)
    {
        this->size = size + 1;
        hh = 0, tt = 0;
        a = new T[size];
    }
    ~queue()
    {
        delete[] a;
    }
    void clear()
    {
        hh = tt;
    }
    bool push(T x)
    {
        if (((tt + 1) % size) == hh)
        {
            qDebug()<<"队满！";
            return false;
        }
        a[tt] = x;
        tt = (tt + 1) % size;
        return true;
    }
    void pop()
    {
        hh = (hh + 1) % size;
    }
    T front()
    {
        return a[hh];
    }
    bool empty()//为空返回true
    {
        return hh == tt;
    }
};
#endif // QUEUE_H
