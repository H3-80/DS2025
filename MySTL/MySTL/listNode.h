#pragma once
#include <cstddef>  // 添加 NULL 定义

typedef int Rank;

template <typename T> struct ListNode;

template <typename T>
using ListNodePosi = ListNode<T>*;

template <typename T> struct ListNode {
    T data;
    ListNodePosi<T> pred;
    ListNodePosi<T> succ;

    ListNode() : data(T()), pred(nullptr), succ(nullptr) {} // 默认构造函数
    ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr)
        : data(e), pred(p), succ(s) {
    }

    ListNodePosi<T> insertAsPred(T const& e) {
        ListNodePosi<T> x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
    }

    ListNodePosi<T> insertAsSucc(T const& e) {
        ListNodePosi<T> x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};