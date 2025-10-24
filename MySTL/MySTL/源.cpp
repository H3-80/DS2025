#pragma once

#include <cstdlib>  // 添加rand()和srand()支持
#include <algorithm> // 添加swap()支持

using std::swap;

// 简单的Fib类实现（替代原Fib.h）
class Fib {
private:
	int f, g; // f = Fib(k), g = Fib(k-1)
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
	Rank _size; int _capacity; T* _elem;	//元素类型为T的数组_elem[]
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	void mergeSort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);

public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)	//容量为c，规模为s，所有元素初始为v
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);	//s<=c
	}	//O(1)
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }	//数组整体设置
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }	//区间复制
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }	//向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }	//区间

	~Vector() { delete[] _elem; }	//释放空间

	//只读接口
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;	//判断向量是否已排序
	Rank find(T const& e) const { return find(e, 0, _size); }	//无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi) const;	//无序向量区间查找
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const;

	//可写接口
	T& operator[] (Rank r) const;	//重载下标操作符，可以类似于数组形式引用各元素(?)
	Vector<T>& operator= (Vector<T> const&);	//重载赋值操作符，以便直接克隆向量(?)
	T remove(Rank r);	//删除秩为r的元素,秩为r的元素对应内部数组的_elem[r],物理地址为_elem + r
	int remove(Rank lo, Rank hi);	//删除秩在区间[lo,hi)之内的元素
	Rank insert(Rank r, T const& e);	//插入元素
	void sort(Rank lo, Rank hi);	//对区间[lo,hi)排序
	void sort() { sort(0, _size); }
	void unsort(Rank lo, Rank hi);	//对区间[lo,hi)置乱
	void unsort() { unsort(0, _size); }
	int deduplicate();
	int uniquify();
	void traverse(void (*) (T&));	//遍历(使用函数指针，只读或局部性修改)(?)
	template <typename VST> void traverse(VST&);	//遍历(使用函数对象，可全局修改)(?)

};

template <typename T>	//元素类型

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V)	//重载“=”(?)
{
	if (_elem) delete[] _elem;	//释放原有内容
	copyFrom(V._elem, 0, V.size());	//整体复制
	return *this;	//返回当前对象的引用，以便链式赋值
}

template <typename T> void Vector<T>::expand()	//向量空间不足时扩容
{
	if (_size < _capacity) return;	//未满员时不扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;	//不低于最小容量
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];	//容量加倍
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];	//复制原向量内容(T为基本类型，或以重载赋值操作符‘=’)
	delete[] oldElem;
}	//insert()运行前先运行expand()函数	O(1)

template <typename T> void Vector<T>::shrink()
{
	if (_capacity < DEFAULT_CAPACITY << 1) return;
	if (_size << 2 > _capacity) return;	//以25%为界
	T* oldElem = _elem; _elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];
	delete[] oldElem;
}	//O(1)

template <typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)	//以区间A[lo,hi)为蓝本复制向量
{
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;	//分配空间(原来空间 * 2)，规模清零
	while (lo < hi)
		_elem[_size++] = A[lo++];	//复制至_elem[0,hi-lo)
}	//忽略开辟新空间所需的时间，O(hi - lo) = O(_size)

template <typename T> T& Vector<T>::operator[] (Rank r) const	//重载下标操作符(?)
{
	return _elem[r];
}

template<typename T> void Vector<T>::unsort(Rank lo, Rank hi)
{
	T* V = _elem + lo;	//将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)(?)
	for (Rank i = hi - lo; i > 0; i--)	//自后向前
		swap(V[i - 1], V[rand() % i]);	//将V[i - 1]与V[0, i)中某一元素随机交换
}

/*
template <typename T> void permute(Vector<T>& V)
{
	for (int i = V._size(); i > 0; i--)
		swap(V[i - 1], V[rand() % i]);
}
*/

//判等器与比较器
template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }	//less than
template <typename T> static bool lt(T& a, T& b) { return a < b; }	//less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); }	//equal
template <typename T> static bool eq(T& a, T& b) { return a == b; }	//equal
template <typename T> static bool mt(T* a, T* b) { return mt(*a, *b); }	//more than
template <typename T> static bool mt(T& a, T& b) { return a > b; }	//more than

template <typename T> int  Vector<T>::disordered() const
{
	int n = 0;
	for (int i = 1; i < _size; i++)	//逐一检查_size - 1对相邻元素
		if (_elem[i - 1] > _elem[i]) n++;
	return n;
}

template <typename T>	//无序向量的
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
	while ((lo < hi--) && (e != _elem[hi]));	//从后向前，顺序查找
	return hi;	//若hi < lo,则意味着失败；否则hi即命中元素的秩
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}

template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)
{
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}

template <typename T> static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi)
{
	Fib fib(hi - lo);
	while (lo < hi) {
		while (hi - lo < fib.get()) fib.prev();
		Rank mi = lo + fib.get() - 1;
		if (e < A[mi]) hi = mi;
		else if (A[mi] < e) lo = mi + 1;
		else return mi;
	}
}

template <typename T>	//将e作为秩为r元素插入
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
	_elem[r] = e; _size++;
	return r;
}	//平均运行时间为O(n)

template <typename T> int Vector<T>::remove(Rank lo, Rank hi)
{
	if (lo == hi) return 0;
	while (hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;	//删除元素的数目
}

template <typename T> T Vector<T>::remove(Rank r)
{
	T e = _elem[r];	//备份
	remove(r, r + 1);
	return e;	//返回被删除元素
}

template <typename T> int Vector<T>::deduplicate()
{
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
	return oldSize - _size;	//被删除元素总数
}	//O(n^2)

template <typename T> void Vector<T>::sort(Rank lo, Rank hi)
{
	switch (rand() % 5) {
	case 1: bubbleSort(lo, hi); break;
	case 2: selectionSort(lo, hi); break;
	case 3: mergeSort(lo, hi); break;
	case 4: heapSort(lo, hi); break;
	case 5: quickSort(lo, hi); break;
	}
}

template <typename T> void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (!bubble(lo, hi--));
}

template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi)
{
	bool sorted = true;
	while (++lo < hi)
		if (_elem[lo - 1] > _elem[lo]) {
			sorted = false; swap(_elem[lo - 1], _elem[lo]);
		}
	return sorted;
}

template <typename T> void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(lo, mi);mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template <typename T> void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
	T* A = _elem + lo;	//A[0,hi - lo) = _elem[lo, hi)
	int lb = mi - lo; T* B = new T[lb];	//B[0,lb) = _elem[lo, hi)
	for (Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi; T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] <= B[j]))) A[i++] = C[j++];
	}
	delete[] B;
}

template <typename T> void Vector<T>::traverse(void (*visit)(T&))	//借助函数指针机制
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}	//遍历向量(?)

template <typename T> template <typename VST>	//元素类型、操作器
void Vector<T>::traverse(VST& visit)	//借助函数对象机制
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}	//遍历向量(?)

