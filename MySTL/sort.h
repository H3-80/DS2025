#pragma once

#include "Vector.h"

// 简化版快速排序 - 使用标准算法
template <typename T>
void quickSort(Vector<T>& vec) {
    if (vec.size() <= 1) return;

    // 使用标准快速排序算法
    int n = vec.size();

    // 如果数据量小，使用插入排序
    if (n <= 16) {
        for (int i = 1; i < n; i++) {
            T key = vec[i];
            int j = i - 1;
            while (j >= 0 && key < vec[j]) {
                vec[j + 1] = vec[j];
                j--;
            }
            vec[j + 1] = key;
        }
        return;
    }

    // 选择基准
    T pivot = vec[n / 2];

    Vector<T> left, middle, right;

    for (int i = 0; i < n; i++) {
        if (vec[i] < pivot) {
            left.insert(left.size(), vec[i]);
        }
        else if (vec[i] > pivot) {
            right.insert(right.size(), vec[i]);
        }
        else {
            middle.insert(middle.size(), vec[i]);
        }
    }

    // 递归排序左右部分
    quickSort(left);
    quickSort(right);

    // 合并结果
    int index = 0;
    for (int i = 0; i < left.size(); i++) {
        vec[index++] = left[i];
    }
    for (int i = 0; i < middle.size(); i++) {
        vec[index++] = middle[i];
    }
    for (int i = 0; i < right.size(); i++) {
        vec[index++] = right[i];
    }
}

// 其他函数保持不变...
template <typename T>
bool majority(Vector<T> A, T& maj) {
    maj = majEleCandidate(A);
    return majEleCheck(A, maj);
}

template <typename T>
bool majEleCheck(Vector<T> A, T maj) {
    int occurrence = 0;
    for (int i = 0; i < A.size(); i++)
        if (A[i] == maj) occurrence++;
    return 2 * occurrence > A.size();
}

template <typename T>
T majEleCandidate(Vector<T> A) {
    T maj;
    for (int c = 0, i = 0; i < A.size(); i++) {
        if (0 == c) {
            maj = A[i]; c = 1;
        }
        else {
            (maj == A[i]) ? c++ : c--;
        }
    }
    return maj;
}

template <typename T>
T trivialMedian(Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2) {
    Vector<T> merged;
    int i = lo1, j = lo2;
    int end1 = lo1 + n1, end2 = lo2 + n2;

    while (i < end1 && j < end2) {
        if (S1[i] < S2[j]) {
            merged.insert(merged.size(), S1[i++]);
        }
        else {
            merged.insert(merged.size(), S2[j++]);
        }
    }

    while (i < end1) {
        merged.insert(merged.size(), S1[i++]);
    }

    while (j < end2) {
        merged.insert(merged.size(), S2[j++]);
    }

    return merged[merged.size() / 2];
}

template <typename T>
T median(Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2) {
    if (n1 > n2) return median(S2, lo2, n2, S1, lo1, n1);
    if (n2 < 6)
        return trivialMedian(S1, lo1, n1, S2, lo2, n2);

    if (2 * n1 < n2)
        return median(S1, lo1, n1, S2, lo2 + (n2 - n1 - 1) / 2, n1 + 2 - (n2 - n1) % 2);

    int mi1 = lo1 + n1 / 2;
    int mi2a = lo2 + (n1 - 1) / 2;
    int mi2b = lo2 + n2 - 1 - n1 / 2;

    if (S1[mi1] > S2[mi2b])
        return median(S1, lo1, n1 / 2 + 1, S2, mi2a, n2 - (n1 - 1) / 2);
    else if (S1[mi1] < S2[mi2a])
        return median(S1, mi1, (n1 + 1) / 2, S2, lo2, n2 - n1 / 2);
    else
        return median(S1, lo1, n1, S2, mi2a, n2 - (n1 - 1) / 2 * 2);
}