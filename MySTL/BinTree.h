#pragma once
#include "BinNode.h"
#include "Stack.h"
#include "Queue.h"
#include <algorithm>
using namespace std;

template <typename T> class BinTree {
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);
    void updateHeightAbove(BinNodePosi(T) x);
public:
    BinTree() : _size(0), _root(NULL) {}
    ~BinTree() { if (0 < _size) remove(_root); }
    int size() const { return _size; }
    bool empty() const { return !_root; }
    BinNodePosi(T) root() const { return _root; }
    BinNodePosi(T) insertAsRoot(T const& e);
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& T);
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& T);
    int remove(BinNodePosi(T) x);
    BinTree<T>* secede(BinNodePosi(T) x);

    template <typename VST>
    void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }
    template <typename VST>
    void travPre(VST& visit) { if (_root) _root->travPre(visit); }
    template <typename VST>
    void travIn(VST& visit) { if (_root) _root->travIn(visit); }
    template <typename VST>
    void travPost(VST& visit) { if (_root) _root->travPost(visit); }
};

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
    while (x) { updateHeight(x); x = x->parent; }
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {
    _size = 1;
    return _root = new BinNode<T>(e);
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi<T> x, T const& e) {
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi<T> x, T const& e) {
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi<T> x, BinTree<T>*& S) {
    if (x->lChild = S->_root) x->lChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    delete S;
    S = NULL;
    return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi<T> x, BinTree<T>*& S) {
    if (x->rChild = S->_root) x->rChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    delete S;
    S = NULL;
    return x;
}

template <typename T>
static int removeAt(BinNodePosi(T) x) {
    if (!x) return 0;
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
    delete x;
    return n;
}

template <typename T>
int BinTree<T>::remove(BinNodePosi<T> x) {
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi<T> x) {
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    BinTree<T>* S = new BinTree<T>;
    S->_root = x;
    x->parent = NULL;
    S->_size = x->size();
    _size -= S->_size;
    return S;
}

// 为BinNode添加遍历方法的实现
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST& visit) {
    Queue<BinNodePosi(T)> Q;
    Q.enqueue(this);
    while (!Q.empty()) {
        BinNodePosi(T) x = Q.dequeue();
        visit(x->data);
        if (HasLChild(*x)) Q.enqueue(x->lChild);
        if (HasRChild(*x)) Q.enqueue(x->rChild);
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST& visit) {
    Stack<BinNodePosi(T)> S;
    BinNodePosi(T) x = this;
    while (true) {
        while (x) {
            visit(x->data);
            if (HasRChild(*x)) S.push(x->rChild);
            x = x->lChild;
        }
        if (S.empty()) break;
        x = S.pop();
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST& visit) {
    Stack<BinNodePosi(T)> S;
    BinNodePosi(T) x = this;
    while (true) {
        while (x) {
            S.push(x);
            x = x->lChild;
        }
        if (S.empty()) break;
        x = S.pop();
        visit(x->data);
        x = x->rChild;
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST& visit) {
    Stack<BinNodePosi(T)> S;
    BinNodePosi(T) x = this;
    BinNodePosi(T) lastVisit = NULL;
    while (x || !S.empty()) {
        while (x) {
            S.push(x);
            x = x->lChild;
        }
        x = S.top();
        if (!x->rChild || x->rChild == lastVisit) {
            visit(x->data);
            S.pop();
            lastVisit = x;
            x = NULL;
        }
        else {
            x = x->rChild;
        }
    }
}

template <typename T>
int BinNode<T>::size() {
    int s = 1;
    if (lChild) s += lChild->size();
    if (rChild) s += rChild->size();
    return s;
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ() {
    BinNodePosi(T) s = this;
    if (rChild) {
        s = rChild;
        while (HasLChild(*s)) s = s->lChild;
    }
    else {
        while (IsRChild(*s)) s = s->parent;
        s = s->parent;
    }
    return s;
}