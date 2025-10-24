#pragma once
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <stdexcept>  // 添加异常处理
#include <cstring>    // 添加内存操作

using std::swap;

class Fib {
private:
    int f, g;
public:
    Fib(int n) {
        f = 1; g = 0;
        while (f < n) next();
    }
    int get() const { return f; }
    void next() { f = f + g; g = f - g; }
    void prev() { g = f - g; f = f - g; }
};

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T> class Vector {
protected:
    Rank _size;
    int _capacity;
    T* _elem;

    void copyFrom(T const* A, Rank lo, Rank hi);
    void expand();
    void shrink();
    bool bubble(Rank lo, Rank hi);
    Rank maxItem(Rank lo, Rank hi);
    void merge(Rank lo, Rank mi, Rank hi);
    Rank partition(Rank lo, Rank hi);
    void heapSort(Rank lo, Rank hi);

public:

    void bubbleSort(Rank lo, Rank hi);
    void selectionSort(Rank lo, Rank hi);
    void mergeSort(Rank lo, Rank hi);
    void quickSort(Rank lo, Rank hi);

    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T()) {
        _capacity = (c > 0) ? c : DEFAULT_CAPACITY;
        _elem = new T[_capacity];
        _size = 0;
        for (; _size < s && _size < _capacity; _size++) {
            _elem[_size] = v;
        }
    }

    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

    ~Vector() { delete[] _elem; }

    Rank size() const { return _size; }
    bool empty() const { return !_size; }

    int disordered() const {
        int n = 0;
        for (int i = 1; i < _size; i++)
            if (_elem[i - 1] > _elem[i]) n++;
        return n;
    }

    Rank find(T const& e) const { return find(e, 0, _size); }

    Rank find(T const& e, Rank lo, Rank hi) const {
        while ((lo < hi--) && !(e == _elem[hi]));
        return hi;
    }

    Rank search(T const& e) const {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }

    Rank search(T const& e, Rank lo, Rank hi) const {
        return binSearch(_elem, e, lo, hi);
    }

    T& operator[](Rank r) const {
        if (r < 0 || r >= _size) {
            throw std::out_of_range("Vector index out of range");
        }
        return _elem[r];
    }

    Vector<T>& operator=(Vector<T> const& V) {
        if (this != &V) {
            if (_elem) delete[] _elem;
            copyFrom(V._elem, 0, V.size());
        }
        return *this;
    }

    T remove(Rank r) {
        T e = _elem[r];
        remove(r, r + 1);
        return e;
    }

    int remove(Rank lo, Rank hi) {
        if (lo == hi) return 0;
        if (lo < 0 || hi > _size || lo > hi) return 0;

        for (Rank i = hi; i < _size; i++) {
            _elem[lo++] = _elem[i];
        }
        _size = lo;
        shrink();
        return hi - lo;
    }

    Rank insert(Rank r, T const& e) {
        if (r < 0 || r > _size) r = _size;
        expand();
        for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
        _elem[r] = e; _size++;
        return r;
    }

    void sort(Rank lo, Rank hi) {
        if (hi - lo < 2) return;
        mergeSort(lo, hi);
    }

    void sort() { sort(0, _size); }

    void unsort(Rank lo, Rank hi) {
        if (lo < 0 || hi > _size || lo >= hi) return;
        T* V = _elem + lo;
        Rank len = hi - lo;
        for (Rank i = len; i > 0; i--) {
            swap(V[i - 1], V[rand() % i]);
        }
    }

    void unsort() { unsort(0, _size); }

    int deduplicate() {
        int oldSize = _size;
        Rank i = 1;
        while (i < _size) {
            (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
        }
        return oldSize - _size;
    }

    int uniquify() {
        if (_size < 2) return 0;
        int oldSize = _size;
        Rank i = 0, j = 1;
        while (j < _size) {
            if (!(_elem[i] == _elem[j])) {
                if (i + 1 != j) {
                    _elem[++i] = _elem[j];
                }
                else {
                    i++;
                }
            }
            j++;
        }
        _size = i + 1;
        shrink();
        return oldSize - _size;
    }

    template <typename VST>
    void traverse(VST& visit) {
        for (int i = 0; i < _size; i++) visit(_elem[i]);
    }

private:
    static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
        while (lo < hi) {
            Rank mi = (lo + hi) >> 1;
            if (e < A[mi]) {
                hi = mi;
            }
            else {
                lo = mi + 1;
            }
        }
        return --lo;
    }
};

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    if (lo > hi) {
        _elem = new T[_capacity = DEFAULT_CAPACITY];
        _size = 0;
        return;
    }

    _capacity = 2 * (hi - lo);
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;

    _elem = new T[_capacity];
    _size = 0;
    while (lo < hi && _size < _capacity) {
        _elem[_size++] = A[lo++];
    }
}

template <typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;

    int newCapacity = (_capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : _capacity * 2;
    T* oldElem = _elem;
    _elem = new T[newCapacity];

    for (int i = 0; i < _size; i++) {
        _elem[i] = oldElem[i];
    }

    _capacity = newCapacity;
    delete[] oldElem;
}

template <typename T>
void Vector<T>::shrink() {
    if (_capacity <= DEFAULT_CAPACITY) return;
    if (_size * 4 > _capacity) return;

    int newCapacity = _capacity / 2;
    if (newCapacity < DEFAULT_CAPACITY) newCapacity = DEFAULT_CAPACITY;

    T* oldElem = _elem;
    _elem = new T[newCapacity];

    for (int i = 0; i < _size; i++) {
        _elem[i] = oldElem[i];
    }

    _capacity = newCapacity;
    delete[] oldElem;
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    for (Rank i = lo + 1; i < hi; i++) {
        if (_elem[i - 1] > _elem[i]) {
            swap(_elem[i - 1], _elem[i]);
            sorted = false;
        }
    }
    return sorted;
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (Rank i = lo + 1; i < hi; i++) {
            if (_elem[i - 1] > _elem[i]) {
                swap(_elem[i - 1], _elem[i]);
                sorted = false;
            }
        }
        hi--;
    }
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo;
    int lb = mi - lo;
    T* B = new T[lb];
    for (int i = 0; i < lb; i++) B[i] = A[i];

    int lc = hi - mi;
    T* C = _elem + mi;

    for (int i = 0, j = 0, k = 0; j < lb || k < lc;) {
        if (j < lb && (k >= lc || B[j] <= C[k])) {
            A[i++] = B[j++];
        }
        if (k < lc && (j >= lb || C[k] < B[j])) {
            A[i++] = C[k++];
        }
    }
    delete[] B;
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    int mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template <typename T>
Rank Vector<T>::maxItem(Rank lo, Rank hi) {
    Rank mx = lo;
    for (Rank i = lo + 1; i < hi; i++) {
        if (_elem[i] > _elem[mx]) mx = i;
    }
    return mx;
}

template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mx = maxItem(lo, hi);
        swap(_elem[--hi], _elem[mx]);
    }
}

template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
    T pivot = _elem[lo];
    while (lo < hi) {
        while (lo < hi && pivot <= _elem[hi]) hi--;
        _elem[lo] = _elem[hi];
        while (lo < hi && _elem[lo] <= pivot) lo++;
        _elem[hi] = _elem[lo];
    }
    _elem[lo] = pivot;
    return lo;
}

template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    Rank mi = partition(lo, hi - 1);
    quickSort(lo, mi);
    quickSort(mi + 1, hi);
}

template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
    selectionSort(lo, hi);
}