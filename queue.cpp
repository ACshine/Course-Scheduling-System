#include"queue.h"
template <class T>
queue<T>::queue(int size) {
    this->size = size + 1;
    hh = 0, tt = 0;
    a = new T[size];
}

template <class T>
queue<T>::~queue() {
    delete[] a;
}

template <class T>
void queue<T>::clear() {
    hh = tt;
}

template <class T>
bool queue<T>::push(T x) {
    if (((tt + 1) % size) == hh) {
        return false;
    }
    a[tt] = x;
    tt = (tt + 1) % size;
    return true;
}

template <class T>
void queue<T>::pop() {
    hh = (hh + 1) % size;
}

template <class T>
T queue<T>::front() {
    return a[hh];
}

template <class T>
bool queue<T>::empty() {
    return hh == tt;
}
