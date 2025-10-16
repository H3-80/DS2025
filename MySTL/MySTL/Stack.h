#pragma once
#include "Vector.h"
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

template <typename T> class Stack : public Vector<T> {
public:
    using Vector<T>::size;
    using Vector<T>::insert;
    using Vector<T>::remove;

    void push(T const& e) { this->insert(this->size(), e); }
    T pop() { return this->remove(this->size() - 1); }
    T& top() { return (*this)[this->size() - 1]; }
};

void convert(Stack<char>& S, long long n, int base) {
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    while (n > 0) {
        int remainder = (int)(n % base);
        S.push(digit[remainder]);
        n /= base;
    }
}

void trim(const char exp[], int& lo, int& hi) {
    while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++;
    while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--;
}

int divide(const char exp[], int lo, int hi) {
    int mi = lo;
    int crc = 1;
    while ((0 < crc) && (++mi < hi)) {
        if (exp[mi] == ')') crc--;
        if (exp[mi] == '(') crc++;
    }
    return mi;
}

bool paren(const char exp[], int lo, int hi) {
    Stack<char> S;
    for (int i = lo; i <= hi; i++) {
        switch (exp[i]) {
        case '(': case '[': case '{': S.push(exp[i]); break;
        case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
        case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
        case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
        default: break;
        }
    }
    return S.empty();
}