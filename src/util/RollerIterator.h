//
// Created by Lukas Tenbrink on 18.02.20.
//

#ifndef LED_FAN_ROLLERITERATOR_H
#define LED_FAN_ROLLERITERATOR_H

#include <iterator>     // std::iterator, std::input_iterator_tag

template <class T>
class RollerIterator : public std::iterator<std::input_iterator_tag, T> {
    const T *s;
    const T *m;
    const T *e;

    T *i;
public:
    // Only for end()
    RollerIterator() : s(nullptr), m(nullptr), e(nullptr), i(nullptr) {}
    RollerIterator(T *s, T *m, T *e, T *i) : s(s), m(m), e(e), i(i) {}
    RollerIterator(T* s, unsigned int head, unsigned int count) : s(s), m(s + head), e(s + count), i(s + head) {}

    RollerIterator &operator++() {
        if (++i == m)
            i = nullptr;
        else if (i == e)
            i = const_cast<T *>(s);

        return *this;
    }

    RollerIterator operator++(int) {
        RollerIterator tmp(s, m, e, i);
        operator++();
        return tmp;
    }

    bool operator==(const RollerIterator &rhs) const { return i == rhs.i; }

    bool operator!=(const RollerIterator &rhs) const { return i != rhs.i; }

    T &operator*() { return *i; }
};


#endif //LED_FAN_ROLLERITERATOR_H
